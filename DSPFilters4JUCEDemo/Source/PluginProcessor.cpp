/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Dspfilters4jucedemoAudioProcessor::Dspfilters4jucedemoAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
bypass(nullptr),
frequency(nullptr)
{
    addParameter(bypass=new AudioParameterBool("bypass","Bypass",false));
    addParameter(frequency=new AudioParameterFloat("frequency","Cutoff",freqRange,2000.0f));
}

//==============================================================================

void Dspfilters4jucedemoAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //  Only process the filter if bypass inactive
    if(*bypass==false)
    {
        //
        //  This is where you use the filter. Just shove an entire
        //  AudioSampleBuffer into the .process() and that's that.
        //  No for(int channel=0;channel<buffer.getNumChannel();++channel)
        //  loop nonsense. Just this line.
        //
        
        filter.process(&buffer,getSampleRate(),*frequency);
        
        //
        //  You can also separate the settings from the filtering by
        //  calling the setup in another part of your program, maybe
        //  prepareToPlay() or a method that updates other filters.
        //
        //      filter.setup(getSampleRate(),*frequency)
        //
        //  After that, you call just the filter with the stream in
        //  process block and continue thinking about something else.
        //
        //      filter.process(&buffer);
        //
    }
    
}

//==============================================================================

const String Dspfilters4jucedemoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Dspfilters4jucedemoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Dspfilters4jucedemoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Dspfilters4jucedemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Dspfilters4jucedemoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Dspfilters4jucedemoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Dspfilters4jucedemoAudioProcessor::setCurrentProgram (int index)
{
}

const String Dspfilters4jucedemoAudioProcessor::getProgramName (int index)
{
    return String();
}

void Dspfilters4jucedemoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Dspfilters4jucedemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Dspfilters4jucedemoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Dspfilters4jucedemoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
AudioProcessorEditor* Dspfilters4jucedemoAudioProcessor::createEditor()
{
    return new Dspfilters4jucedemoAudioProcessorEditor (*this);
}

//==============================================================================
void Dspfilters4jucedemoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Dspfilters4jucedemoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Dspfilters4jucedemoAudioProcessor();
}
