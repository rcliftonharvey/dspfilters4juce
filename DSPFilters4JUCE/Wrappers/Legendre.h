//  *----------------------------------------------------------------------------*
//
//  DSPFilters optimized for JUCE
//
//  *----------------------------------------------------------------------------*
//
//  "A Collection of Useful C++ Classes for Digital Signal Processing"
//
//  Original DSPFilters library by Vinnie Falco
//  https://github.com/vinniefalco/DSPFilters
//  Copyright (c) 2009 by Vinnie Falco
//
//  Adapted for Linux by Bernd Porr
//  https://github.com/berndporr/iir1
//  Copyright (c) 2012 by Bernd Porr
//
//  Optimized for JUCE by Rob Clifton-Harvey
//  https://github.com/rc-h/dspfilters4juce
//  Copyright (c) 2017 by Rob Clifton-Harvey
//
//  *----------------------------------------------------------------------------*
//  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//  *----------------------------------------------------------------------------*

#ifndef WRAPPER_LEGENDRE_H_INCLUDED
#define WRAPPER_LEGENDRE_H_INCLUDED

//
//  LEGENDRE FILTER
//  ===============
//
//    Supported DSPFILTERS::FilterTypes
//    ---------------------------------
//
//      LOWPASS
//      HIGHPASS
//      BANDPASS
//      BANDSTOP
//
//
//  Don't change this. Srsly tho.
#define CUR_FILT_NAME DSPFILTERS::FilterNames::LEGENDRE
//
//  Default filter type and order if template arguments not given (feel free to change)
#define CUR_DFLT_TYPE DSPFILTERS::FilterTypes::LOWPASS
#define CUR_DFLT_ORDR 1
// ------------------------------------------------------------------------------------- //

template<DSPFILTERS::FilterTypes Type=CUR_DFLT_TYPE, unsigned int Order=CUR_DFLT_ORDR>
class Legendre : public SKELETON<CUR_FILT_NAME,Type,Order>, private WRAPPER<CUR_FILT_NAME,Type,Order>
{
public:
    
    Legendre () {}
    ~Legendre () {}
    
    //
    //  PARAMETER SETTER and GETTER methods
    //
    //  Setter methods allow you to change individual filter settings,
    //  rather than always calling the humongous setup() one-liners.
    //
    //  Getter methods allow you to poll this filter's parameter values,
    //  quite convenient if you want to poll them for your GUI from the
    //  PluginEditor.cpp.
    //
    //  CAREFUL:
    //    Not all filters behave alike. Where the Width on one may be a Q
    //    value, the Width on another one may be another frequency. Inform.
    //
    //  Every wrapper will publish the methods fitting for its filter kind.
    //
    void setSampleRate  (double SR)     {SKELETON<CUR_FILT_NAME,Type,Order>::setSampleRate(SR);}
    void setFrequency   (double Hz)     {SKELETON<CUR_FILT_NAME,Type,Order>::setFrequency(Hz);}
    void setWidth       (double Width)  {SKELETON<CUR_FILT_NAME,Type,Order>::setWidth(Width);}
    
    double getSampleRate () {return SKELETON<CUR_FILT_NAME,Type,Order>::getSampleRate();}
    double getFrequency ()  {return SKELETON<CUR_FILT_NAME,Type,Order>::getFrequency();}
    double getWidth ()      {return SKELETON<CUR_FILT_NAME,Type,Order>::getWidth();}
    
    // --------------------------------------------------------------------------------- //
    //
    //          SETUP ROUTINES
    //
    // --------------------------------------------------------------------------------- //
    //
    //  Use these to update the filter's settings.
    //
    //  Don't be afraid to call these often, the filters
    //  will only reset if the values actually change.
    //
    //  Passing more parameters than *stream into process()
    //  will call setup() before processing.
    //
    // --------------------------------------------------------------------------------- //
    //
    //  EXAMPLE:
    //
    //     DSPFILTERS::Legendre<> filter;           @ PluginProcessor.h file !!!
    //
    //     filter.setup(44100.0,2000.0);            @ PluginProcessor.cpp (anywhere)
    //
    //  -> creates LPF at 2 kHz at SR 44.1 kHz
    //
    // --------------------------------------------------------------------------------- //
    
    //  quick one
    void setup (double SR, double Hz)
    {
        SKELETON<CUR_FILT_NAME,Type,Order>::setup(SR,Hz,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterWidth,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterGain,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterRipple,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterRolloff);
    }
    
    // universal one
    void setup (double SR, double Hz, double Width)
    {
        SKELETON<CUR_FILT_NAME,Type,Order>::setup(SR,Hz,
                                                  Width,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterGain,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterRipple,
                                                  SKELETON<CUR_FILT_NAME,Type,Order>::filterRolloff);
    }
    
    
    // --------------------------------------------------------------------------------- //
    //
    //          PROCESSING ROUTINES
    //
    // --------------------------------------------------------------------------------- //
    //
    //  Use these to let an entire AudioSampleBuffer be filtered.
    //
    //  Call anywhere in processBlock() to apply.
    //
    // --------------------------------------------------------------------------------- //
    //
    //  EXAMPLE:
    //
    //     DSPFILTERS::Legendre<> filter;           @ PluginProcessor.h file !!!
    //
    //     filter.process(&buffer);                 @ PluginProcessor.cpp (anywhere)
    //     ---- or ----
    //     filter.process(&buffer,44100.0,2000.0);
    //
    // --------------------------------------------------------------------------------- //
    
    //  process only -- make sure the filter is set up correctly!
    void process (AudioSampleBuffer* stream)
    {
        SKELETON<CUR_FILT_NAME,Type,Order>::process(stream);
    }
    
    //  quick one
    void process (AudioSampleBuffer* stream, double SR, double Hz)
    {
        SKELETON<CUR_FILT_NAME,Type,Order>::process(stream,SR,Hz,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterWidth,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterGain,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterRipple,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterRolloff);
    }
    
    //  universal one
    void process (AudioSampleBuffer* stream, double SR, double Hz, double Width)
    {
        SKELETON<CUR_FILT_NAME,Type,Order>::process(stream,SR,Hz,
                                                    Width,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterGain,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterRipple,
                                                    SKELETON<CUR_FILT_NAME,Type,Order>::filterRolloff);
    }
    
};


// Leave these here to avoid compiler warnings
#undef CUR_FILT_NAME
#undef CUR_DFLT_TYPE
#undef CUR_DFLT_ORDR

#endif // WRAPPER_LEGENDRE_H_INCLUDED
