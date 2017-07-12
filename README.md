# DSPFilters for JUCE

-------------------------------------------------------------------------------------------------------

## Index

1. [Introduction](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#introduction)
2. [Important](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#important)
3. [Description](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#description)
4. [How to use](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#how-to-use)
5. [Example project](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#example-project)
6. [Notes](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#notes)
7. [License](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#license)
8. [Credits](https://github.com/rcliftonharvey/dspfilters4juce/blob/master/README.md#credits)

-------------------------------------------------------------------------------------------------------

## Introduction 

This is a powerful filter library which implements all standard IIR filters such as Bessel, Butterworth, Elliptic and Chebychev. The data format is floating-point throughout.

This library has been adapted from Bernd Porr's Linux port of Vinnie Falco's **DSPFilters**.

Vinnie Falco's original **DSPFilters** library can be found here:<br>
[https://github.com/vinniefalco/DSPFilters]

Bernd Porr's Linux port of it can be found here:<br>
[https://github.com/berndporr/iir1]

Rob Clifton-Harvey's JUCE optimized version (this one) is kept here:<br>
[https://github.com/rcliftonharvey/dspfilters4juce]

-------------------------------------------------------------------------------------------------------

## Important

I created this version of **DSPFilters** to be used with the [JUCE](https://www.juce.com/) framework. All the wrapper classes rely on functionality in JUCE, you'll have a *seriously* hard time trying to get this to run without it...

-------------------------------------------------------------------------------------------------------

## Description

The Vinnie Falco and Bernd Porr versions were fine, but the filters needed to be calculated on a per-sample basis, which I am not a fan of. Having *for-every-channel* and *for-every-sample* loops over and over in ***processBlock*** is just clutter in the one place where there shouldn't be any clutter.

I also don't enjoy setting up single filters for every channel, especially when they're all the same kind and it's not clear right away how many channels my processor will have to handle. Or worse, having to set up arrays for them and managing to keep track of all their various states...

There are some multi-channel and per-buffer methods that are part of the original **DSPFilters** code, I am aware of those. However, they are meant to operate on float arrays which are not "native" to JUCE, as JUCE uses its own data type [AudioSampleBuffer](https://www.juce.com/doc/classAudioBuffer).

Since JUCE's **AudioSampleBuffer** is capable of many more things than float arrays, I have created wrapper classes that will make them easier to process. This allows the code to stay "native" to JUCE and it eliminates the need for loops and float array extraction.

These wrappers automatically instantiate as many filters as there are channels in the passed **AudioSampleBuffer**, and loop through it while applying the filters to all the contained sample data.

#### Remember this?
```c++
filter1.setup(getSampleRate(),filterFreq);
filter2.setup(getSampleRate(),filterFreq);

for(unsigned int channel=0;channel<buffer.getNumChannels();++channel)
{
  float* channelData=buffer.getWritePointer(channel);
  
  for(unsigned int sample=0;sample<buffer.getNumSamples();++sample)
  {
    if      (channel==0) { channelData[sample]=filter1.process(sample); }
    else if (channel==1) { channelData[sample]=filter2.process(sample); }
  }
}
```

#### Forget it. Now you can do it all in one line:
```c++
filter.process(&buffer,getSampleRate(),filterFreq);
```

No more *for* loops, no more filter counting, no more temporary float arrays.<br>
What more can you wish for? :)

-------------------------------------------------------------------------------------------------------

## How to use

Add all the files from the [DSPFilters4JUCE](https://github.com/rcliftonharvey/dspfilters4juce/tree/master/DSPFilters4JUCE) folder into a group in your JUCE project. The easiest way to do this is via the files pane in the Projucer app. Then add this line to the beginning your **PluginProcessor.h**:
```c++
#include "DSPFilters.h"
```

That's all to get you started.
The filters are now available in the **DSP::** namespace. (Unless you changed it.)

Instantiate a filter wrapper class in **PluginProcessor.h**:
```c++
DSP::RBJ<DSP::LOWPASS,2> lpf;
```

This will create a *2nd order Biquad low-pass* based on the *RBJ* formulae.

*The notation for this step is identical for all filters. First you pick the filter design, then you specify a filter type and lastly you give it an order. The higher the order is, the steeper the things should get.* ***Here's a universal example:***
```c++
DSP::FilterDesign<FilterType,Order> variableName;
```

  *I will add a list of all available designs and their possible types.*

Now that your filter is instantiated, you need to tell it what sample rate and frequency to operate at, otherwise it can't process correctly. Set the filter up somewhere in **PluginProcessor.cpp**, maybe ***prepareToPlay*** or even ***processBlock***:
```c++
lpf.setup(getSampleRate(),cutoffFreq);
```

Finally send an **AudioSampleBuffer** into it in ***processBlock***:
```c++
lpf.process(&buffer);
```

If you're lazy like me, you may want to do all that in a *single line* in ***processBlock***:
```c++
lpf.process(&buffer,sampleRate,cutoffFreq,etc.);
```

Doing this will update the filter's settings first, then process the buffer with it.

*Don't worry about constantly pushing values into the process() method, the filter wrappers are designed to only ever bother the filters about coefficient updates when the values actually change. If you send the same sample rate and cutoff frequency into process() 200 times and they're always the same, the filters won't care. If the sample rate or cutoff change on the 201st time, then the filters will update.*

-------------------------------------------------------------------------------------------------------

## Example project

For a real world example, check out the [DSPFilters4JUCEDemo](https://github.com/rcliftonharvey/dspfilters4juce/tree/master/DSPFilters4JUCEDemo) project.

-------------------------------------------------------------------------------------------------------

## Notes

I have omitted lots of the originally included data of the former distributions, like demo applications and the original DSPFilters Documentation. They are not required or "fitting" for this port anymore, but you can always obtain them through the links to the original repositories of [Vinnie](https://github.com/vinniefalco/DSPFilters) and [Bernd](https://github.com/berndporr/iir1).

Because Bernd Porr's adadptation changes the structure so deeply, like renaming class methods and removing the entire Design aspect, it's impossible to use this JUCE-ified version with the original DSPFilters library "out of the box". I tried it, but eventually had to resign. It would mean making such numerous edits to the original DSPFilters library, that I would've had to include a customized version with this one anyway.

If you plan on using this and one of the formers in the same project, then be sure to put this one into another namespace than **Dsp** (Falco) or **Iir** (Porr). The default namespace for this library is **DSP**.

-------------------------------------------------------------------------------------------------------

## License

The source code is provided under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).<br>
A copy of the MIT license in written form comes in the download of this library.

The JUCE framework itself is licensed separately, see the [JUCE website](https://juce.com) for more information.

-------------------------------------------------------------------------------------------------------

## Credits

The original **DSP Filters** library is Copyright &copy; 2009 by Vinnie Falco<br>
The **Linux** adaptation of it is Copyright &copy; 2012 by Bernd Porr<br>
This **JUCE** optimized version is Copyright &copy; 2017 Rob Clifton-Harvey

-------------------------------------------------------------------------------------------------------

Enjoy!

Rob Clifton-Harvey
