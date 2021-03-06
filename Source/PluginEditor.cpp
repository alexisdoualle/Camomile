/*
 // Copyright (c) 2015-2018 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginEnvironment.h"
#include "PluginLookAndFeel.hpp"
#include "Pd/PdPatch.hpp"

CamomileEditor::CamomileEditor(CamomileAudioProcessor& p) :
AudioProcessorEditor (&p), CamomileEditorInteractionManager(p), m_processor (p), m_button(p)
{
    static CamoLookAndFeel lookAndFeel;
    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);
    
    setOpaque(true);
    setWantsKeyboardFocus(true);
    setInterceptsMouseClicks(true, true);
    auto const bounds = p.getPatch().getBounds();
    setSize(bounds[2] > 0 ? std::max(bounds[2], 100) : 400, bounds[3] > 0 ? std::max(bounds[3], 100) : 300);
    Image const& img = CamoLookAndFeel::getImage();
    if(img.isValid())
    {
        m_image.setImage(img);
        m_image.setTransformToFit(getBounds().toType<float>(), RectanglePlacement::stretchToFit);
        addAndMakeVisible(m_image, 0);
    }
    else if(!CamomileEnvironment::getImageName().empty())
    {
        p.add(CamomileAudioProcessor::ConsoleLevel::Error,
                      "background image " + CamomileEnvironment::getImageName() + " is invalid or doesn't exist.");
    }
    
    for(auto& gui : p.getPatch().getGuis()) {
        addAndMakeVisible(m_objects.add(PluginEditorObject::createTyped(*this, gui))); }
    addAndMakeVisible(m_button);
    startTimer(25);
}

CamomileEditor::~CamomileEditor() {}

void CamomileEditor::timerCallback()
{
    CamomileEditorPanelManager::processMessages();
    for(auto object : m_objects) {
        object->update(); }
}


void CamomileEditor::paint (Graphics& g)
{
    g.fillAll(Colours::white);
    if(!CamomileEnvironment::isValid())
    {
        g.setColour(Colours::black);
        g.drawText("Plugin Not Valid", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
    else if(!m_processor.getPatch().isGraph())
    {
        g.setColour(Colours::black);
        g.drawText("No Graphical User Interface Available", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool CamomileEditor::keyPressed(const KeyPress& key) {
    return CamomileEditorKeyManager::keyPressed(key); }

bool CamomileEditor::keyStateChanged(bool isKeyDown) {
    return CamomileEditorKeyManager::keyStateChanged(isKeyDown); }

void CamomileEditor::modifierKeysChanged(const ModifierKeys& modifiers) {
    CamomileEditorKeyManager::keyModifiersChanged(modifiers); }




