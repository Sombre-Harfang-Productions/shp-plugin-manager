#pragma once

#include <JuceHeader.h>
#include "PluginInfo.h"

// Scrollable release-notes window. Call ChangelogDialog::show() from any component.
class ChangelogDialog : public juce::Component
{
public:
    ChangelogDialog (const std::vector<ChangelogEntry>& changelog);

    void resized() override;

    // Builds and launches a floating DialogWindow. Caller provides window title.
    static void show (const juce::String& windowTitle,
                      const std::vector<ChangelogEntry>& changelog);

private:
    juce::TextEditor editor;
};
