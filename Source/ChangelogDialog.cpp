#include "ChangelogDialog.h"
#include "Theme.h"

using namespace shp::theme;

ChangelogDialog::ChangelogDialog (const std::vector<ChangelogEntry>& changelog)
{
    juce::String text;

    if (changelog.empty())
    {
        text = "(aucune note de version disponible)";
    }
    else
    {
        for (const auto& entry : changelog)
        {
            text << "v" << entry.version;
            if (entry.date.isNotEmpty())
                text << "   " << entry.date;
            text << "\n"
                 << juce::String (std::string (44, '-').c_str())
                 << "\n";
            text << (entry.body.isNotEmpty() ? entry.body : "(pas de notes)") << "\n\n";
        }
    }

    editor.setMultiLine (true);
    editor.setReadOnly (true);
    editor.setScrollbarsShown (true);
    editor.setFont (monoFont (11.0f));
    editor.setText (text.trimEnd(), false);
    editor.setColour (juce::TextEditor::backgroundColourId, surfaceDeep);
    editor.setColour (juce::TextEditor::textColourId,       bone);
    editor.setColour (juce::TextEditor::outlineColourId,    railDark);
    editor.setColour (juce::TextEditor::shadowColourId,     juce::Colours::transparentBlack);
    addAndMakeVisible (editor);

    setSize (520, 400);
}

void ChangelogDialog::resized()
{
    editor.setBounds (getLocalBounds().reduced (8));
}

void ChangelogDialog::show (const juce::String& windowTitle,
                            const std::vector<ChangelogEntry>& changelog)
{
    auto* content = new ChangelogDialog (changelog);

    juce::DialogWindow::LaunchOptions opts;
    opts.content.setOwned (content);
    opts.dialogTitle                  = windowTitle;
    opts.dialogBackgroundColour       = background;
    opts.escapeKeyTriggersCloseButton = true;
    opts.useNativeTitleBar            = false;
    opts.resizable                    = false;
    opts.launchAsync();
}
