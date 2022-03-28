#!/bin/bash

USAGE="Please run in the same directory as the Presets folder without sudo."

if [[ $EUID == 0 ]]; then
    echo You are running this script as root.
    echo $USAGE
    exit 1
fi

if [[ ! -d "./Presets" ]]; then
    echo "No ./Presets folder found."
    echo $USAGE
    exit 1
fi

PRESETS=`pwd`/Presets

cd ~/Documents

if [[ ! -d "Lumatone Editor" ]]; then
    echo 'Creating "~/Documents/Lumatone Editor/"'
    mkdir "Lumatone Editor" && chmod 777 "Lumatone Editor" && FOLDERCREATED=1
    if [[ ! $FOLDERCREATED ]]; then
        echo "Error creating '~/Documents/Lumatone Editor', try creating it manually and run this again."
        exit 1
    fi
fi

echo "Copying preset files..."
cp -n -r ${PRESETS}/* "Lumatone Editor" && FILESCOPIED=1
if [[ ! $FILESCOPIED ]]; then
    echo 'Sorry, there was a problem copying files. Just move the "Mappings" & "Palettes" folders from ./Presets into "~/Documents/Lumatone Editor/" to manually install.'
    exit 1
fi

chmod -R 777 "Lumatone Editor"

echo "Preset installation successful! Feel free to delete the local Presets folder."
echo
echo "Before running Lumatone Editor, you must install the dependency \"libssh2\"."
echo
echo "For debian-based distros use:"
echo "    sudo apt install libssh2"
echo
echo "For arch-based distros use:"
echo "    sudo pacman -S libssh2"
echo
echo "Or alternatively whatever equivalent command for the package manager you use."
exit 0
