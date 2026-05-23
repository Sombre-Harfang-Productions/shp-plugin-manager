#pragma once

#include <JuceHeader.h>
#include "PluginInfo.h"

class PluginCardComponent : public juce::Component
{
public:
    explicit PluginCardComponent (PluginInfo info);

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseEnter (const juce::MouseEvent&) override;
    void mouseExit  (const juce::MouseEvent&) override;

    void setRowIndex (int idx) { rowIndex = idx; repaint(); }

    std::function<void (const PluginInfo&)> onAction;

private:
    void showChangelog();

    PluginInfo info;
    int rowIndex  { 0 };
    bool hovered  { false };

    juce::TextButton actionButton;
    juce::TextButton manualButton;
    juce::TextButton changelogButton;
    juce::Image logo;
};
