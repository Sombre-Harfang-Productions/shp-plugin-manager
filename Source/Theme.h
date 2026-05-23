#pragma once

#include <JuceHeader.h>

namespace shp::theme
{
// Base surfaces
inline const auto background  = juce::Colour::fromRGB (10,  10,  11);   // #0a0a0b
inline const auto surface     = juce::Colour::fromRGB (13,  13,  15);   // #0d0d0f header bg
inline const auto surfaceDeep = juce::Colour::fromRGB (8,   8,   11);
inline const auto moduleA     = juce::Colour::fromRGB (19,  19,  22);   // #131316 even rows
inline const auto moduleB     = juce::Colour::fromRGB (16,  16,  19);   // #101013 odd rows
inline const auto moduleHover = juce::Colour::fromRGB (26,  26,  30);   // #1a1a1e hover
inline const auto iconBg      = juce::Colour::fromRGB (26,  26,  30);   // #1a1a1e icon square bg
inline const auto iconBorder  = juce::Colour::fromRGB (37,  37,  42);   // #25252a icon border

// Rails / borders
inline const auto railDark    = juce::Colour::fromRGB (31,  31,  35);   // #1f1f23 dividers
inline const auto railLight   = juce::Colour::fromRGB (46,  46,  50);   // #2e2e32 button borders

// Text
inline const auto bone        = juce::Colour::fromRGB (232, 223, 200);  // #e8dfc8 primary
inline const auto dimBone     = juce::Colour::fromRGB (122, 116, 104);  // #7a7468 secondary
inline const auto dust        = juce::Colour::fromRGB (90,  86,  80);   // #5a5650 muted
inline const auto dimPlugin   = juce::Colour::fromRGB (174, 166, 147);  // #aea693 not-installed name

// Status / action accents
inline const auto updateAmber  = juce::Colour::fromRGB (216, 184, 138); // #d8b88a
inline const auto installGreen = juce::Colour::fromRGB (168, 201, 177); // #a8c9b1
inline const auto statusGreen  = juce::Colour::fromRGB (138, 171, 146); // #8aab92 installed dot
inline const auto dangerText   = juce::Colour::fromRGB (154, 147, 137); // #9a9389 uninstall

// Legacy (error states)
inline const auto blood       = juce::Colour::fromRGB (190, 18,  32);
inline const auto bloodBright = juce::Colour::fromRGB (245, 34,  48);
inline const auto bloodDeep   = juce::Colour::fromRGB (82,  5,   12);

inline juce::Font monoFont (float height, int flags = 0)
{
    return juce::Font (juce::FontOptions ("JetBrains Mono", height, flags)
        .withFallbacks ({ "Consolas", "Cascadia Mono", juce::Font::getDefaultMonospacedFontName() }));
}

inline juce::Font oswaldFont (float height, int flags = juce::Font::bold, float tracking = 0.0f)
{
    auto font = juce::Font (juce::FontOptions ("Oswald", height, flags)
        .withFallbacks ({ "Arial Narrow", "Segoe UI", juce::Font::getDefaultSansSerifFontName() }));
    font.setExtraKerningFactor (tracking);
    return font;
}
}
