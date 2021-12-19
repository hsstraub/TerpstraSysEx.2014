/*
  ==============================================================================

    PathBrowserComponent.h
    Created: 5 Sep 2020 9:59:39pm
    Author:  Vincenzo

    Simple combination of a text editor and button to trigger browsing for a 
    directory and then displaying the full directory path.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PathBrowserComponent : public Component, public Button::Listener
{
public:

    PathBrowserComponent(const String dialogBoxTitle,
                         String fileTypeFilter,
                         File fileIn = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory))
    {
        setName(dialogBoxTitle);

        chooser = std::make_unique<FileChooser>(dialogBoxTitle, fileIn, fileTypeFilter);

        editor.reset(new TextEditor(dialogBoxTitle + "Editor"));
        editor->setMultiLine(false, false);
        editor->setReadOnly(true);
        addAndMakeVisible(editor.get());

        openButton.reset(new TextButton(dialogBoxTitle + "Button", "Choose file for " + dialogBoxTitle));
        openButton->setButtonText("...");
        openButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
        addAndMakeVisible(openButton.get());
        openButton->addListener(this);
    }

    ~PathBrowserComponent()
    {
        editor = nullptr;
        openButton = nullptr;
    }


    void resized() override
    {
        openButton->setSize(getHeight(), getHeight());
        openButton->setTopRightPosition(getWidth() - 1, 0);
        editor->setBounds(0, 0, getWidth() - getHeight(), getHeight());
    }

    void buttonClicked(Button* buttonThatWasClicked) override
    {
        chooser->launchAsync(FileBrowserComponent::FileChooserFlags::openMode, 
            [&](const FileChooser& chooser)
            {
                auto result = chooser.getResult();
                if (result.existsAsFile())
                {
                    editor->setText(chooser.getResult().getFullPathName(), false);
                    listeners.call(&PathBrowserComponent::Listener::fileChanged, this, chooser.getResult());
                }
            });
    }

    TextEditor* getEditor() { return editor.get(); }

    TextButton* getButton() { return openButton.get(); }

    void setText(String textIn)
    {
        editor->setText(textIn);
    }

    class Listener
    {
    public:
        
        Listener() {};
        virtual ~Listener() {};

        virtual void fileChanged(PathBrowserComponent* source, File fileSelected) = 0;
    };

    void addListener(PathBrowserComponent::Listener* listenerIn)
    {
        listeners.add(listenerIn);
    }

    void removeListener(PathBrowserComponent::Listener* listenerIn)
    {
        listeners.remove(listenerIn);
    }

protected:

    ListenerList<PathBrowserComponent::Listener> listeners;

private:

    std::unique_ptr<FileChooser> chooser;

    std::unique_ptr<TextEditor> editor;
    std::unique_ptr<TextButton> openButton;

    std::function<void(const File&)> directorySetCallback;
};
