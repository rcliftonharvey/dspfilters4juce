/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//
//  Make sure that you've added all the files from the DSPFilters4JUCE folder into a group of your
//  JUCE project. The easiest way to do this is via the Projucer app.
//
//  Then just include the DSPFilters header here.
//
#include "../../DSPFilters4JUCE/DSPFilters.h"

//
//  You now have access to the filters. By default, they will be in the DSP:: namespace. Should you
//  wish to change the namespace, look at the file DSPFilters.h and modify the last bit of this line:
//
//  #define DSPFILTERS DSP
//
//  If you change DSP to something else, like RCH or Filters, then that's the namespace where all the
//  filters are going to be. To access them, just type DSP:: and let your IDE's autocomplete guide you.
//
//  Should your IDE not have an intelligent autocomplete feature, here's the list of available filters:
//
//      DSP::Bessel<>
//      DSP::Butterworth<>
//      DSP::ChebyshevI<>
//      DSP::ChebyshevII<>
//      DSP::Elliptic<>
//      DSP::Legendre<>
//      DSP::RBJ<>
//


//==============================================================================
/**
*/
class Dspfilters4jucedemoAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Dspfilters4jucedemoAudioProcessor();
    ~Dspfilters4jucedemoAudioProcessor() {}

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override {return false;}  // -- No fancy GUI, sorry -- //

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    //
    //  This is where to instantiate the filter.
    //
    //  In this specific case, we're after a 4th order Butterworth lowpass.
    //
    //  There are lots of other filter types available, they can all be
    //  instantiated and used in the same way, even the RBJ Biquads.
    //
    //  See the Readme for more information.
    //
    DSP::Butterworth<DSP::LOWPASS,4> filter;
    
    //
    //  This is just a quick-n-dirty solution to get Hz values out of the
    //  float parameter without doing lots of math. In a real product,
    //  you'd obviously do this better. Right?
    //
    NormalisableRange<float> freqRange=NormalisableRange<float>(20.0f,20000.0f,1.0f,0.35f,false);
    
    //
    //  Just two parameters that let your control the plugin from the host.
    //
    AudioParameterBool*     bypass;         // Bypasses the filter if engaged
    AudioParameterFloat*    frequency;      // Filter cutoff 20 to 20K Hz
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dspfilters4jucedemoAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
