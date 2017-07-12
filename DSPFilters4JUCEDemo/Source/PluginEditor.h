/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Dspfilters4jucedemoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Dspfilters4jucedemoAudioProcessorEditor (Dspfilters4jucedemoAudioProcessor&);
    ~Dspfilters4jucedemoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Dspfilters4jucedemoAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dspfilters4jucedemoAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
