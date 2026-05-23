#include "ShpLookAndFeel.h"
#include "Theme.h"

using namespace shp::theme;

ShpLookAndFeel::ShpLookAndFeel()
{
    setColour (juce::ResizableWindow::backgroundColourId, background);
    setColour (juce::Label::textColourId,                 bone);
    // Default = ghost button: transparent bg, dimBone text
    setColour (juce::TextButton::buttonColourId,          juce::Colours::transparentBlack);
    setColour (juce::TextButton::buttonOnColourId,        juce::Colours::transparentBlack);
    setColour (juce::TextButton::textColourOffId,         bone.withAlpha (0.86f));
    setColour (juce::TextButton::textColourOnId,          bone.withAlpha (0.86f));
    setColour (juce::ScrollBar::thumbColourId,            dust.withAlpha (0.55f));
    setColour (juce::ScrollBar::trackColourId,            surfaceDeep);
}

void ShpLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                           juce::Button& button,
                                           const juce::Colour&,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    const auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    const bool lit    = shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown;

    // Fill — use buttonColourId set per-button (transparent for ghost, tinted for update/install)
    auto bgColour = button.findColour (juce::TextButton::buttonColourId);
    if (lit) bgColour = bgColour.brighter (0.12f);
    g.setColour (bgColour);
    g.fillRoundedRectangle (bounds, 4.0f);

    // Border — ghost buttons get railLight; tinted buttons get text colour at 0.45
    const bool hasBg = bgColour.getAlpha() > 10;
    if (hasBg)
    {
        auto textCol = button.findColour (juce::TextButton::textColourOffId);
        g.setColour (textCol.withAlpha (lit ? 0.75f : 0.45f));
    }
    else
    {
        g.setColour (railLight.withAlpha (lit ? 0.85f : 0.55f));
    }
    g.drawRoundedRectangle (bounds, 4.0f, 1.0f);
}

void ShpLookAndFeel::drawButtonText (juce::Graphics& g,
                                     juce::TextButton& button,
                                     bool,
                                     bool)
{
    g.setFont (monoFont (10.5f));
    g.setColour (button.findColour (juce::TextButton::textColourOffId));
    g.drawFittedText (button.getButtonText().toUpperCase(),
                      button.getLocalBounds().reduced (6, 2),
                      juce::Justification::centred,
                      1);
}
