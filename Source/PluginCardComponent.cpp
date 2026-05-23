#include "PluginCardComponent.h"
#include "ChangelogDialog.h"
#include "Theme.h"
#include <BinaryData.h>

using namespace shp::theme;

static int textPxWidth (const juce::Font& f, const juce::String& s)
{
    juce::GlyphArrangement ga;
    ga.addLineOfText (f, s, 0.0f, 0.0f);
    return juce::roundToInt (ga.getBoundingBox (0, -1, true).getWidth());
}

// Layout constants (pixels)
namespace
{
constexpr int kIconX      = 10;
constexpr int kIconSize   = 36;
constexpr int kActionsW   = 200;   // rightmost zone: buttons
constexpr int kBadgeW     = 148;   // status badge zone
constexpr int kVersionW   = 114;   // version text zone
}

PluginCardComponent::PluginCardComponent (PluginInfo i)
    : info (std::move (i))
{
    logo = juce::ImageCache::getFromMemory (BinaryData::shp_logo_v3_png,
                                            BinaryData::shp_logo_v3_pngSize);

    // Action button (Install / Update / Uninstall)
    if (info.hasAction())
    {
        addAndMakeVisible (actionButton);

        if (info.status == PluginInfo::Status::updateAvailable)
        {
            actionButton.setButtonText ("UPDATE");
            actionButton.setColour (juce::TextButton::buttonColourId,  updateAmber.withAlpha (0.10f));
            actionButton.setColour (juce::TextButton::textColourOffId, updateAmber);
        }
        else if (info.status == PluginInfo::Status::notInstalled)
        {
            actionButton.setButtonText ("INSTALL");
            actionButton.setColour (juce::TextButton::buttonColourId,  installGreen.withAlpha (0.10f));
            actionButton.setColour (juce::TextButton::textColourOffId, installGreen);
        }
        else  // upToDate → Uninstall (danger ghost)
        {
            actionButton.setButtonText ("UNINSTALL");
            actionButton.setColour (juce::TextButton::textColourOffId, dangerText);
        }

        actionButton.onClick = [this] { if (onAction) onAction (info); };
    }

    // Manual button
    if (info.manualUrl.isNotEmpty())
    {
        addAndMakeVisible (manualButton);
        manualButton.setButtonText ("MANUAL");
        manualButton.setColour (juce::TextButton::textColourOffId, bone.withAlpha (0.86f));
        manualButton.onClick = [this] { juce::URL (info.manualUrl).launchInDefaultBrowser(); };
    }

    // Changelog / notes button
    if (! info.changelog.empty())
    {
        addAndMakeVisible (changelogButton);
        changelogButton.setButtonText ("NOTES");
        changelogButton.setColour (juce::TextButton::textColourOffId, bone.withAlpha (0.86f));
        changelogButton.onClick = [this] { showChangelog(); };
    }
}

void PluginCardComponent::mouseEnter (const juce::MouseEvent&)
{
    hovered = true;
    repaint();
}

void PluginCardComponent::mouseExit (const juce::MouseEvent&)
{
    hovered = false;
    repaint();
}

void PluginCardComponent::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds();

    // ── Row background ───────────────────────────────────────────────────────
    g.setColour (hovered ? moduleHover
                         : (rowIndex % 2 == 0 ? moduleA : moduleB));
    g.fillRoundedRectangle (bounds.toFloat(), 6.0f);

    // ── Icon square ──────────────────────────────────────────────────────────
    const int iconY = (getHeight() - kIconSize) / 2;
    const auto iconRect = juce::Rectangle<int> (kIconX, iconY, kIconSize, kIconSize);

    g.setColour (iconBg);
    g.fillRoundedRectangle (iconRect.toFloat(), 4.0f);
    g.setColour (iconBorder);
    g.drawRoundedRectangle (iconRect.toFloat(), 4.0f, 1.0f);

    if (logo.isValid())
    {
        const auto logoRect = iconRect.reduced (8);
        g.saveState();
        if (info.status == PluginInfo::Status::notInstalled)
            g.setOpacity (0.40f);
        g.drawImage (logo, logoRect.toFloat(), juce::RectanglePlacement::centred);
        g.restoreState();
    }

    // ── Name + description ───────────────────────────────────────────────────
    const int nameX = kIconX + kIconSize + 14;
    const int nameW = getWidth() - nameX - kVersionW - kBadgeW - kActionsW;

    const int textBlockH = 15 + 3 + 12;
    const int textY = (getHeight() - textBlockH) / 2;

    g.setFont (oswaldFont (13.0f, juce::Font::bold, 0.08f));
    g.setColour (info.status == PluginInfo::Status::notInstalled ? dimPlugin : bone);
    g.drawText (info.name.toUpperCase(),
                juce::Rectangle<int> (nameX, textY, nameW, 15),
                juce::Justification::centredLeft, true);

    g.setFont (monoFont (10.0f));
    g.setColour (dimBone);
    g.drawText (info.description,
                juce::Rectangle<int> (nameX, textY + 15 + 3, nameW, 12),
                juce::Justification::centredLeft, true);

    // ── Version column ───────────────────────────────────────────────────────
    const int versionX = getWidth() - kActionsW - kBadgeW - kVersionW;
    const auto versionRect = juce::Rectangle<int> (versionX, 0, kVersionW, getHeight());
    g.setFont (monoFont (10.5f));

    switch (info.status)
    {
        case PluginInfo::Status::upToDate:
            g.setColour (dimBone);
            g.drawText ("v" + info.installedVersion, versionRect,
                        juce::Justification::centredRight, true);
            break;

        case PluginInfo::Status::updateAvailable:
        {
            // "v0.2.1  →  v0.2.3" — old version muted, new version amber
            const juce::String arrow = juce::String::fromUTF8 ("\xe2\x86\x92");
            const juce::String oldPart = "v" + info.installedVersion + "  " + arrow + "  ";
            const juce::String newPart = "v" + info.latestVersion;
            const auto f = monoFont (10.5f);
            const int oldW = textPxWidth (f, oldPart);
            const int newW = textPxWidth (f, newPart);
            const int totalW = oldW + newW;
            const int startX = versionRect.getRight() - totalW;
            const int cy = (getHeight() - 13) / 2;

            g.setColour (dust);
            g.drawText (oldPart,
                        juce::Rectangle<int> (startX, cy, oldW + 2, 13),
                        juce::Justification::centredLeft, false);
            g.setColour (updateAmber);
            g.drawText (newPart,
                        juce::Rectangle<int> (startX + oldW, cy, newW + 4, 13),
                        juce::Justification::centredLeft, false);
            break;
        }

        case PluginInfo::Status::notInstalled:
            g.setColour (dust);
            g.drawText ("v" + info.latestVersion + " available", versionRect,
                        juce::Justification::centredRight, true);
            break;

        default:
            break;
    }

    // ── Status badge (pill) ──────────────────────────────────────────────────
    const int badgeX = getWidth() - kActionsW - kBadgeW;
    const auto badgeAreaRect = juce::Rectangle<int> (badgeX, 0, kBadgeW, getHeight());

    juce::Colour badgeBg, badgeFg, badgeBorder;
    juce::String badgeLabel;

    switch (info.status)
    {
        case PluginInfo::Status::upToDate:
            badgeBg     = statusGreen.withAlpha (0.14f);
            badgeFg     = installGreen;
            badgeBorder = statusGreen.withAlpha (0.40f);
            badgeLabel  = "INSTALLED";
            break;
        case PluginInfo::Status::updateAvailable:
            badgeBg     = updateAmber.withAlpha (0.12f);
            badgeFg     = updateAmber;
            badgeBorder = updateAmber.withAlpha (0.40f);
            badgeLabel  = "UPDATE AVAILABLE";
            break;
        case PluginInfo::Status::notInstalled:
            badgeBg     = dust.withAlpha (0.08f);
            badgeFg     = dangerText;
            badgeBorder = dust.withAlpha (0.30f);
            badgeLabel  = "NOT INSTALLED";
            break;
        default:
            badgeBg     = dust.withAlpha (0.08f);
            badgeFg     = dust;
            badgeBorder = dust.withAlpha (0.25f);
            badgeLabel  = info.statusLabel();
            break;
    }

    const auto badgeFont  = monoFont (9.5f);
    const int  dotSize    = 5;
    const int  dotGap     = 6;
    const int  hPad       = 10;
    const int  pillTextW  = textPxWidth (badgeFont, badgeLabel);
    const int  pillW      = hPad + dotSize + dotGap + pillTextW + hPad;
    const int  pillH      = 22;
    const auto pillRect   = juce::Rectangle<int> (badgeAreaRect.getCentreX() - pillW / 2,
                                                   (getHeight() - pillH) / 2,
                                                   pillW, pillH);

    g.setColour (badgeBg);
    g.fillRoundedRectangle (pillRect.toFloat(), 11.0f);
    g.setColour (badgeBorder);
    g.drawRoundedRectangle (pillRect.toFloat(), 11.0f, 1.0f);

    // Dot
    const int dotX = pillRect.getX() + hPad;
    const int dotY = pillRect.getCentreY() - dotSize / 2;
    g.setColour (badgeFg.withAlpha (0.85f));
    g.fillEllipse ((float) dotX, (float) dotY, (float) dotSize, (float) dotSize);

    // Label
    g.setFont (badgeFont);
    g.setColour (badgeFg);
    g.drawText (badgeLabel,
                juce::Rectangle<int> (dotX + dotSize + dotGap, pillRect.getY(),
                                      pillTextW + 4, pillH),
                juce::Justification::centredLeft, false);
}

void PluginCardComponent::resized()
{
    const bool hasAct = actionButton.isVisible();
    const bool hasMn  = manualButton.isVisible();
    const bool hasCl  = changelogButton.isVisible();

    // Actions zone: rightmost kActionsW px, centred vertically at 28px tall
    auto row = getLocalBounds()
                   .removeFromRight (kActionsW)
                   .withSizeKeepingCentre (kActionsW, 28);

    if (hasAct)
    {
        const int actW = (info.status == PluginInfo::Status::upToDate) ? 78 : 68;
        actionButton.setBounds (row.removeFromRight (actW));
        if (hasMn || hasCl) row.removeFromRight (4);
    }
    if (hasMn)
    {
        manualButton.setBounds (row.removeFromRight (68));
        if (hasCl) row.removeFromRight (4);
    }
    if (hasCl)
    {
        changelogButton.setBounds (row.removeFromRight (58));
    }
}

void PluginCardComponent::showChangelog()
{
    ChangelogDialog::show (info.name.toUpperCase() + " — RELEASE NOTES",
                           info.changelog);
}
