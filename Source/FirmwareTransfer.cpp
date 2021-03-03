/*
  ==============================================================================

    FirmwareTransfer.cpp
    Created: 11 Feb 2021 2:36:11am
    Author:  Vincenzo

  ==============================================================================
*/

#include "FirmwareTransfer.h"
#include "Main.h"
#if JUCE_WINDOWS
	#include "Networking/curl-7.75.0-win64-mingw/include/curl/curl.h"
	#include "Networking/libssh2-1.9.0-win64-mingw/include/libssh2.h"
#endif



FirmwareTransfer::FirmwareTransfer(TerpstraMidiDriver& driverIn)
	: midiDriver(driverIn)
{

}

FirmwareTransfer::~FirmwareTransfer()
{

}

static int waitForSSHSocket(int socket_fd, LIBSSH2_SESSION* session)
{
	struct timeval timeout;
	int rc;
	fd_set fd;
	fd_set* writefd = NULL;
	fd_set* readfd = NULL;
	int dir;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&fd);

	FD_SET(socket_fd, &fd);

	/* now make sure we wait in the correct direction */
	dir = libssh2_session_block_directions(session);


	if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
		readfd = &fd;

	if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		writefd = &fd;

	rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

	return rc;
}

static int shutdownSSHSession(LIBSSH2_SESSION* session, int sock, FILE* localFile, int returnCode)
{
	DBG("Shutting down...");
	while (libssh2_session_disconnect(session, "Normal Shutdown,") == LIBSSH2_ERROR_EAGAIN);
	libssh2_session_free(session);

#ifdef WIN32
	closesocket(sock);
#else
	close(sock);
#endif
	if (localFile)
		fclose(localFile);
	DBG("all done\n");

	libssh2_exit();

	return returnCode;
}

// adapted from:
// https://www.libssh2.org/examples/scp_write_nonblock.html
// https://www.libssh2.org/examples/ssh2_exec.html
int FirmwareTransfer::initializeFirmwareUpdate(String firmwareFilePath)
{
	if (!File::isAbsolutePath(firmwareFilePath))
	{
		DBG("FIRMWARE: Input file is not a valid path");
		return 1;
	}
	
	// Shared SSH session data
	const char* deviceHostName = SERVERHOST;
	unsigned long hostaddr = inet_addr(deviceHostName);
	const char* username = SERVERKEY;
	const char* password = SERVERPWD;

	int sock, i, auth_pw = 1;
	struct sockaddr_in sin;
	const char* fingerprint;
	
	LIBSSH2_SESSION* session = NULL;
	LIBSSH2_CHANNEL* channel;
	
	// SCP Transfer
	String serverFilePath = "/home/" + String(SERVERKEY) + "/" + File(firmwareFilePath).getFileName(); // JUCE convenience
	const char* targetPath = serverFilePath.getCharPointer();
	const char* filePath = firmwareFilePath.getCharPointer();
	struct stat fileinfo;
	FILE* localFile;
	char fileBuffer[1024];
	size_t numBytesRead;
	char* bufferPtr;


	// SSH Exec
	String rebootCmd = "echo " + String(password) + " | sudo -S -p \"\" reboot";

    int exitcode;
    char *exitsignal = (char *)"none";
    int bytecount = 0;
    size_t len;
    LIBSSH2_KNOWNHOSTS *nh;
    int type;


#ifdef WIN32
	WSADATA wsadata;
	int err;

	err = WSAStartup(MAKEWORD(2, 0), &wsadata);
	if (err != 0) {
		DBG("WSAStartup failed with error: " + String(err));
		return (int)ErrorCode::StartupErr;
	}
#endif

	int returnCode = libssh2_init(0);

	if (returnCode != 0) 
	{
		DBG("libssh2 initialization failed! error: " + String(returnCode));
		return (int)ErrorCode::StartupErr;
	}

	// Prepare data for file transfer
	localFile = fopen(filePath, "rb");
	if (!localFile) 
	{
		DBG("failed to open file!");
		return (int)ErrorCode::StartupErr;
	}

	stat(filePath, &fileinfo);

	// Create socket and connect to port 22
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		DBG("failed to create socket!");
		return (int)ErrorCode::StartupErr;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) 
	{
		DBG("failed to connect!");
		return (int)ErrorCode::HostConnectErr;
	}

    /* Create a session instance */ 
	session = libssh2_session_init();
	if (!session)
	{
		DBG("failed to create session!");
		return (int)ErrorCode::HostConnectErr;
	}

	/* tell libssh2 we want it all done non-blocking */ 
    libssh2_session_set_blocking(session, 0);


	/* ... start it up. This will trade welcome banners, exchange keys,
	 * and setup crypto, compression, and MAC layers
	 */
	while((returnCode = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN);
	if (returnCode) 
	{
		DBG("Failure establishing SSH session, libssh2 error: " + String(returnCode));
		return (int)ErrorCode::SessionEstErr;
	}

	/* At least for now, disregard "known hosts" since we're using a private, local network over USB 
	 * Now, we must properly close the session regardless of outcome
	 */
	
	// Authenticate via password
	while((returnCode = libssh2_userauth_password(session, username, password)) == LIBSSH2_ERROR_EAGAIN);
	if (returnCode != 0)
	{
		DBG("Authentication by password failed.\n");
		return shutdownSSHSession(session, sock, localFile, (int)ErrorCode::AuthErr);
	}

	// BEGIN FIRMWARE FILE TRANSFER

	/* Send a file via scp. The mode parameter must only have permissions! */
	do {
		channel = libssh2_scp_send(session, targetPath, fileinfo.st_mode & 0777, (unsigned long)fileinfo.st_size);
		
		if ((!channel) && (libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN))
		{
			char* errmsg;
			libssh2_session_last_error(session, &errmsg, NULL, 0);
			DBG("Unable to open a session, libssh2 error: " + String(errmsg));
			return shutdownSSHSession(session, sock, localFile, (int)ErrorCode::ChannelErr);
		}

	} while (!channel);

	DBG("SCP session waiting to send file\n");
	do {
		numBytesRead = fread(fileBuffer, 1, sizeof(fileBuffer), localFile);
		if (numBytesRead <= 0) {
			/* end of file */
			break;
		}
		bufferPtr = fileBuffer;

		do {
			/* write the same data over and over, until error or completion */
			while((returnCode = libssh2_channel_write(channel, bufferPtr, numBytesRead)) == LIBSSH2_ERROR_EAGAIN) waitForSSHSocket(sock, session);

			if (returnCode < 0) {
				DBG("Error writing to channel, libssh2 error: " + String(returnCode));
				break;
			}
			else {
				/* returnCode indicates how many bytes were written this time */
				bufferPtr += returnCode;
				numBytesRead -= returnCode;
			}
		} while (numBytesRead);
	} while (!numBytesRead); /* only continue if numBytesRead was drained */ 

	DBG("Sending EOF");
	while((returnCode = libssh2_channel_send_eof(channel)) == LIBSSH2_ERROR_EAGAIN);

	DBG("Waiting for EOF");
	while ((returnCode = libssh2_channel_wait_eof(channel)) == LIBSSH2_ERROR_EAGAIN);

	DBG("Waiting for channel to close");
	while ((returnCode = libssh2_channel_wait_closed(channel)) == LIBSSH2_ERROR_EAGAIN);;

	while ((returnCode = libssh2_channel_free(channel)) == LIBSSH2_ERROR_EAGAIN);;


	// REQUEST REBOOT FOR FIRMWARE INSTALL

	/* Exec non-blocking on the remove host */ 
    while((channel = libssh2_channel_open_session(session))  == NULL &&
	      libssh2_session_last_error(session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN) 
	{
		waitForSSHSocket(sock, session);
    }
    if(channel == NULL) 
	{
        DBG("Error opening channel for reboot request");
		return shutdownSSHSession(session, sock, nullptr, (int)ErrorCode::ChannelErr);
    }
    
	while((returnCode = libssh2_channel_exec(channel, rebootCmd.getCharPointer())) == LIBSSH2_ERROR_EAGAIN) 
	{
		waitForSSHSocket(sock, session);
    }
    if(returnCode != 0) 
	{
        DBG("Error preparing channel for command execution ");
        return shutdownSSHSession(session, sock, nullptr, (int)ErrorCode::ExecChnlErr);
    }

    for(;;)
	{
        /* loop until we block */ 
        int channelReturnCode;
        do {
            char buffer[0x4000];
            channelReturnCode = libssh2_channel_read(channel, buffer, sizeof(buffer));

            if(channelReturnCode > 0) 
			{
                int i;
                bytecount += channelReturnCode;
                for(i = 0; i < channelReturnCode; ++i)
                    fputc(buffer[i], stderr);
            }
            else 
			{
                if(channelReturnCode != LIBSSH2_ERROR_EAGAIN)
                    /* no need to output this for the EAGAIN case */ 
                    DBG("libssh2_channel_read returned: " + String(channelReturnCode));
            }
        }
        while(channelReturnCode > 0);
 
        /* this is due to blocking that would occur otherwise so we loop on
           this condition */ 
        if(channelReturnCode == LIBSSH2_ERROR_EAGAIN) {
			waitForSSHSocket(sock, session);
        }
        else
            break;
    }
    exitcode = 127;
    while((returnCode = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		waitForSSHSocket(sock, session);
 
    if(returnCode == 0) 
	{
        exitcode = libssh2_channel_get_exit_status(channel);
        libssh2_channel_get_exit_signal(channel, &exitsignal,
                                        NULL, NULL, NULL, NULL, NULL);
    }
 
    if(exitsignal)
        DBG("Got signal: " + String(exitsignal));
    else
        DBG("Exit: " + String(exitcode) + "bytecount: " + String(bytecount));
 
    libssh2_channel_free(channel);

    channel = NULL;

	return shutdownSSHSession(session, sock, nullptr, (int)ErrorCode::NoErr);
}

int FirmwareTransfer::requestFirmwareDownloadAndUpdate()
{
	// TODO download firmware file
	File firmwareFile = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile("zeta.py");

	int updateCode = initializeFirmwareUpdate(firmwareFile.getFullPathName().getCharPointer());
	bool fileDeleted = false;//.firmwareFile.deleteFile();

	return updateCode;
}

void FirmwareTransfer::midiMessageReceived(const MidiMessage& midiMessage)
{

}

void FirmwareTransfer::midiMessageSent(const MidiMessage& midiMessage)
{

}

void FirmwareTransfer::midiSendQueueSize(int queueSize)
{

}

void FirmwareTransfer::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{

}