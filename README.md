This is a powerful filter library which implements all standard IIR filters such as
Bessel, Butterworth, Elliptic and Chebychev. The data format is floating-point throughout.

There is no need to resort to MATLAB/OCTAVE to calculate the filter coefficients because
the library does it by itself. Just provide the sampling rate, cutoff frequency, filter
order and the filter is ready to be used.

This library has been adapted from Bernd Porr's Linux port of Vinnie Falco's DSPFilters.

Vinnie Falco's original DSPFilters can be found here:
https://github.com/vinniefalco/DSPFilters

Bernd Porr's Linux port of it can be found here:
https://github.com/berndporr/iir1

Rob Clifton-Harvey's version (this) is kept here:
https://github.com/rc-h/dspfilters4juce

-------------------------------------------------------------------------------------------------------

Important
=========

I created his version of DSPFilters to be used with the JUCE framework. All the wrapper classes rely
on functionality in JUCE, you'll have a SERIOUSLY hard time trying to get this to run without it...

-------------------------------------------------------------------------------------------------------

The Vinnie Falco and Bernd Porr versions were fine, but I found them a bit tedious because filters will
be calculated on a per-sample basis, which I am not a fan of. Having "for every channel" and "for every
sample" loops over and over in processBlock() to me just clutters the one place where you should design
the flow of your signal. Plus I don't enjoy setting up single filters for every channel, and especially 
so when it's not clear right away how many channels my processor will have to handle.

There are some multi-channel and per-buffer methods that are part of the original DSPFilters code, I am
aware. However, those are meant to operate on float arrays which are not "native" to JUCE, as JUCE uses
its own data type AudioSampleBuffer.

Since JUCE's AudioSampleBuffers are capable of much more than mere float arrays, I have created wrapper
classes that will make AudioSampleBuffers easier to process.

The wrappers will automatically instantiate as many filters as are channels in the Buffer that is about
to be processed, as well as loop through the Buffer and apply the filters to all the samples on all the
channels inside it.

Simply instantiate one of my wrapper classes in the header of the PluginProcessor, then call the .setup
for it (w/ arguments) and pass an AudioSampleBuffer into the .process() method, that's it.

I added convenient one-liner .process() calls which let you do the setup and the processing all in one.
See further down for an example.

-------------------------------------------------------------------------------------------------------

Please note that my wrappers were designed for easy use, they may not allow access to the full range of
functionality or the full extent of flexibility the DSPFilters actually offer. If you feel like there's
room for improvement, or if you know how to further extend my version's functionality, feel free to get
in touch and let me know.

-------------------------------------------------------------------------------------------------------

I have modified some of the source files, simply to keep things neat and easy. I did counter an "unused
variable" compiler warning, apart from that the DSP remains unchanged by me. The copyright & credit for
the DSP code still belongs to Vinnie Falco.

-------------------------------------------------------------------------------------------------------

I have omitted lots of the originally included data of the former distributions, like demo applications
and the original DSPFilters Documentation. They are not required or "fitting" for this port anymore but
you can always obtain them through the links to the original repositories of Vinnie and Bernd above.

-------------------------------------------------------------------------------------------------------

Because Bernd Porr's adadptation changes aspects of the structure, like renaming process() to filter(),
it's impossible to use this JUCE-ified version with the original DSPFilters library "out of the box". I
tried making it interchangeable, but eventually had to resign. It would mean making such numerous edits
to the original DSPFilters library, that I would've had to include the customized version with this one
anyway.

If you plan on using this and one of the formers in the same project, then be sure to put this one into
another namespace than Dsp (Falco) or Iir (Porr).

-------------------------------------------------------------------------------------------------------

How to use:
===========

Add all the files from the DSPFilters4JUCE folder into a group of your JUCE project. The easiest way to
do this is via the Projucer app. Then add this line to the beginning your PluginProcessor.h:

 #include "DSPFilters.h"

That's all, the filters are now available in the DSP:: namespace. (Unless you changed it.)

Instantiate a wrapper class in PluginProcessor.h:

 Dsp::Biquad<LOWPASS,2> lpf;

Set the filter up somewhere in PluginProcessor.cpp, maybe prepareToPlay or even processBlock:

 lpf.setup(sampleRate,cutoffFreq,etc.);             // note that arguments vary from filter to filter

Finally send an AudioSampleBuffer into it in processBlock:

 lpf.process(&buffer);

If you're lazy like me, you may want to do all that in a single line in processBlock:

 lpf.process(&buffer,sampleRate,cutoffFreq,etc.);

-------------------------------------------------------------------------------------------------------

Don't worry about updating the settings all the time, the filters are smart enough to not always really
update and reset. They'll only do that if a parameter value has in fact changed.

-------------------------------------------------------------------------------------------------------

 For a real world example, check the /demo source.

-------------------------------------------------------------------------------------------------------

Enjoy :)

Rob Clifton-Harvey

2017-07-11
