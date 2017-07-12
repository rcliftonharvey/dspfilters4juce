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

#ifndef WRAPPERS_H_INCLUDED
#define WRAPPERS_H_INCLUDED

// ---------------------------------------------- //
//
//  DON'T CHANGE THESE!
//  FILTER WRAPPERS USE THEM AND DEPEND ON THEM!
//
// ---------------------------------------------- //
#define SKELETON DSPFILTERS::Wrappers::Skeleton   //
#define WRAPPER  DSPFILTERS::Wrappers::Wrapper    //
// ---------------------------------------------- //

namespace Wrappers
{
    //
    //  The Skeleton class provides the structure for the AudioSampleBuffer based filter wrappers.
    //
    //  Forward-declaring this so it can be 'friend' with Wrapper class.
    //  Scroll down past Wrapper class for full Skeleton declaration.
    //
    template <DSPFILTERS::FilterNames Name, DSPFILTERS::FilterTypes Type, int Order>
    class Skeleton;
    
// ---------------------------------------------------------------------------------------------------------------------------------------------------- //
// ------------------------------- SINGLE CHANNEL FILTERS --------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------------------------------------- //
    
    //
    //  WRAPPER CLASS
    //
    //  This acts as a middle man between the original DspFilters and
    //  the JUCE optimized wrappers. It basically instantiates several
    //  filters and turns them into better maintainable 'abstracts'.
    //
    //  To the best of my knowledge, filters will NOT take up any
    //  noticeable or even measurable amounts of memory or CPU just
    //  from being instantiated. It's the processing and the maths
    //  that chews up all the performance. As ever only one filter
    //  per instance of the Wrapper class is calculated - no threat.
    //
    //  Everything in here is still per-sample.
    //
    //  The Skeleton class further down turns these 'abstract' single
    //  channel filters into autonomous AudioSampleBuffer processors.
    //
    template <DSPFILTERS::FilterNames Name, DSPFILTERS::FilterTypes Type, int Order>
    class Wrapper
    {
    public:
        
        Wrapper () {}
        virtual ~Wrapper () {}
        
        //  Stuff needs to be protected so it doesn't 'bleed through'.
        //
        //  Making Skeleton class a friend of Wrapper class so Skeleton
        //  can access Wrapper's protected area. ICYWW: for .setup() and
        //  .filter() access.
        friend SKELETON<Name,Type,Order>;
        
    protected:
        
        //
        //  UPDATES the SETTINGS for this filter, called from Skeleton::process()
        //
        //  As these single channel filters are 'hidden' from direct use anyway,
        //  there's no need to have individual set/get functionality. A huge
        //  one-liner will do just fine.
        //
        //  If the passed values are the same as the filter's current settings,
        //  nothing will happen.
        //
        //  Only if any of the passed values vary from the filter's current
        //  settings, the coeffsUpdated flag will be set. In the next process()
        //  if this flag is TRUE, filters will be reset and their values updated.
        //
        //  This is a protected function, you don't need to access it directly.
        //
        virtual void setup (double SR, double Hz, double Width, double dB, double Ripple, double Rolloff)
        {
            if(SR!=filterSampleRate)    {filterSampleRate=SR;coeffsUpdated=true;}
            if(Hz!=filterFrequency)     {filterFrequency=Hz;coeffsUpdated=true;}
            if(Width!=filterWidth)      {filterWidth=Width;coeffsUpdated=true;}
            if(dB!=filterGain)          {filterGain=dB;coeffsUpdated=true;}
            if(Ripple!=filterRipple)    {filterRipple=Ripple;coeffsUpdated=true;}
            if(Rolloff!=filterRolloff)  {filterRolloff=Rolloff;coeffsUpdated=true;}
        }
        
        //
        //  The actual PER SAMPLE processing of a SINGLE FILTER instance.
        //
        //  This is never directly called from the wrappers, it's merely
        //  a 'convenience split' to avoid redundancy, and to have better
        //  readable chunks of code.
        //
        //  This is also protected and inaccessible. If you want to have
        //  simple single channel filters that you can control, just use
        //  the originals without the wrapper overhead.
        //
        //  They're in the DSPFILTERS::Templates namespace.
        //
        double processSample (double input)
        {
            double processed=0.0;
            
            updateFilters();
            
            jassert(filterSampleRate>0.0);
            jassert(filterFrequency>0.0);

            /* Bessel Filters */
            if      ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::LOWPASS))  {processed=bessel.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::HIGHPASS)) {processed=bessel.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::LOWSHELF)) {processed=bessel.LS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::BANDPASS)) {processed=bessel.BP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::BANDSTOP)) {processed=bessel.BS.filter(input);}
            /* Butterworth Filters */
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {processed=bworth.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {processed=bworth.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {processed=bworth.LS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {processed=bworth.HS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {processed=bworth.BP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {processed=bworth.BS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {processed=bworth.SH.filter(input);}
            /* ChebyshevI Filters */
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {processed=cshev1.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {processed=cshev1.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {processed=cshev1.LS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {processed=cshev1.HS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {processed=cshev1.BP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {processed=cshev1.BS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {processed=cshev1.SH.filter(input);}
            /* ChebyshevII Filters */
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {processed=cshev2.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {processed=cshev2.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {processed=cshev2.LS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {processed=cshev2.HS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {processed=cshev2.BP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {processed=cshev2.BS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {processed=cshev2.SH.filter(input);}
            /* Elliptic Filters */
            else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {processed=ellipt.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {processed=ellipt.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {processed=ellipt.BP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {processed=ellipt.BS.filter(input);}
            /* Legendre Filters */
            else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {processed=legend.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {processed=legend.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {processed=legend.BP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {processed=legend.BS.filter(input);}
            /* Robert Bristow-Johnson Filters */
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {processed=rbjohn.LP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {processed=rbjohn.HP.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {processed=rbjohn.LS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {processed=rbjohn.HS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {processed=rbjohn.BP1.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDPASS2)) {processed=rbjohn.BP2.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {processed=rbjohn.BS.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {processed=rbjohn.SH.filter(input);}
            else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::ALLPASS))   {processed=rbjohn.AP.filter(input);}

            return processed;
        }
        
        //  This is the one-line process() method the Skeleton class will actually call.
        double processSample (double input, double SR, double Hz, double Width, double dB, double Ripple, double Rolloff)
        {
            setup(SR,Hz,Width,dB,Ripple,Rolloff);
            return processSample(input);
        }
        
    private:
        
        //
        //  ALL TEH FILTERZ
        //
        //  I know it looks weird and doesn't seem "clean".
        //  But it's fine. Really. Just enjoy the symmetry.
        //
        //  OCD heaven. :)
        //
        struct BesselFilters
        {
            DSPFILTERS::Templates::Bessel::LowPass<Order>  LP;
            DSPFILTERS::Templates::Bessel::HighPass<Order> HP;
            DSPFILTERS::Templates::Bessel::LowShelf<Order> LS;
            DSPFILTERS::Templates::Bessel::BandPass<Order> BP;
            DSPFILTERS::Templates::Bessel::BandStop<Order> BS;
        } bessel;
        
        struct ButterworthFilters
        {
            DSPFILTERS::Templates::Butterworth::LowPass<Order>   LP;
            DSPFILTERS::Templates::Butterworth::HighPass<Order>  HP;
            DSPFILTERS::Templates::Butterworth::LowShelf<Order>  LS;
            DSPFILTERS::Templates::Butterworth::HighShelf<Order> HS;
            DSPFILTERS::Templates::Butterworth::BandPass<Order>  BP;
            DSPFILTERS::Templates::Butterworth::BandStop<Order>  BS;
            DSPFILTERS::Templates::Butterworth::BandShelf<Order> SH;
        } bworth;
        
        struct ChebyshevIFilters
        {
            DSPFILTERS::Templates::ChebyshevI::LowPass<Order>   LP;
            DSPFILTERS::Templates::ChebyshevI::HighPass<Order>  HP;
            DSPFILTERS::Templates::ChebyshevI::LowShelf<Order>  LS;
            DSPFILTERS::Templates::ChebyshevI::HighShelf<Order> HS;
            DSPFILTERS::Templates::ChebyshevI::BandPass<Order>  BP;
            DSPFILTERS::Templates::ChebyshevI::BandStop<Order>  BS;
            DSPFILTERS::Templates::ChebyshevI::BandShelf<Order> SH;
        } cshev1;
        
        struct ChebyshevIIFilters
        {
            DSPFILTERS::Templates::ChebyshevII::LowPass<Order>   LP;
            DSPFILTERS::Templates::ChebyshevII::HighPass<Order>  HP;
            DSPFILTERS::Templates::ChebyshevII::LowShelf<Order>  LS;
            DSPFILTERS::Templates::ChebyshevII::HighShelf<Order> HS;
            DSPFILTERS::Templates::ChebyshevII::BandPass<Order>  BP;
            DSPFILTERS::Templates::ChebyshevII::BandStop<Order>  BS;
            DSPFILTERS::Templates::ChebyshevII::BandShelf<Order> SH;
        } cshev2;
        
        struct EllipticFilters
        {
            DSPFILTERS::Templates::Elliptic::LowPass<Order>   LP;
            DSPFILTERS::Templates::Elliptic::HighPass<Order>  HP;
            DSPFILTERS::Templates::Elliptic::BandPass<Order>  BP;
            DSPFILTERS::Templates::Elliptic::BandStop<Order>  BS;
        } ellipt;
        
        struct LegendreFilters
        {
            DSPFILTERS::Templates::Legendre::LowPass<Order>   LP;
            DSPFILTERS::Templates::Legendre::HighPass<Order>  HP;
            DSPFILTERS::Templates::Legendre::BandPass<Order>  BP;
            DSPFILTERS::Templates::Legendre::BandStop<Order>  BS;
        } legend;
        
        //
        //  RBJArray class used below is a bunch of per-sample RBJs.
        //
        //  Since the RBJ Biquads didn't have an "order", I created
        //  the RBJArray helper class that will process a number of
        //  RBJ filters of identical types serially, one after the
        //  other. This gets us our "order" and filter conformity.
        //
        //  See RBJArray.h for the full mess.
        //
        struct CookbookFilters
        {
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::LOWPASS,Order>   LP;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::HIGHPASS,Order>  HP;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::LOWSHELF,Order>  LS;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::HIGHSHELF,Order> HS;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::BANDPASS,Order>  BP1;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::BANDPASS2,Order> BP2;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::BANDSTOP,Order>  BS;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::BANDSHELF,Order> SH;
            DSPFILTERS::Templates::RBJArray<DSPFILTERS::FilterTypes::ALLPASS,Order>   AP;
        } rbjohn;
        
        bool coeffsUpdated=false;
        
        //
        //  FILTER SETTINGS
        //
        //  These values are passed down through setup() from the
        //  Skeleton class.
        //
        //  They all need a default value, shouldn't be undefined.
        //
        double filterSampleRate=0.0;    // in Hz
        double filterFrequency=0.0;     // in Hz
        double filterWidth=0.0;         // in Hz
        double filterGain=0.0;          // in dB
        double filterRipple=0.0;        // in dB
        double filterRolloff=0.0;       // in dB
        
        //
        //  If any settings have changed, re-calculate this filter's settings.
        //  If no settings have changed, nothing happens.
        //
        //  Make sure this is run EVERY processSample() BEFORE the filters
        //  actually process something, otherwise the filters won't know what
        //  to do, or possibly do the wrong thing. Which is never a good idea.
        //
        void updateFilters ()
        {
            if (coeffsUpdated==true)
            {
                //
                //  REMEMBER to .reset() the filters before updating their coefficients
                //  Doing no resets can lead to SUPER loud peaks... so just do them.
                //

                /* Bessel Filters */
                if      ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::LOWPASS))  {bessel.LP.reset();bessel.LP.setup(Order,filterSampleRate,filterFrequency);}
                else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::HIGHPASS)) {bessel.HP.reset();bessel.HP.setup(Order,filterSampleRate,filterFrequency);}
                else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::LOWSHELF)) {bessel.LS.reset();bessel.LS.setup(Order,filterSampleRate,filterFrequency,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::BANDPASS)) {bessel.BP.reset();bessel.BP.setup(Order,filterSampleRate,filterFrequency,filterWidth);}
                else if ((Name==DSPFILTERS::FilterNames::BESSEL) and (Type==DSPFILTERS::FilterTypes::BANDSTOP)) {bessel.BS.reset();bessel.BS.setup(Order,filterSampleRate,filterFrequency,filterWidth);}
                /* Butterworth Filters */
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {bworth.LP.reset();bworth.LP.setup(Order,filterSampleRate,filterFrequency);}
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {bworth.HP.reset();bworth.HP.setup(Order,filterSampleRate,filterFrequency);}
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {bworth.LS.reset();bworth.LS.setup(Order,filterSampleRate,filterFrequency,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {bworth.HS.reset();bworth.HS.setup(Order,filterSampleRate,filterFrequency,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {bworth.BP.reset();bworth.BP.setup(Order,filterSampleRate,filterFrequency,filterWidth);}
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {bworth.BS.reset();bworth.BS.setup(Order,filterSampleRate,filterFrequency,filterWidth);}
                else if ((Name==DSPFILTERS::FilterNames::BUTTERWORTH) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {bworth.SH.reset();bworth.SH.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterGain);}
                /* ChebyshevI Filters */
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {cshev1.LP.reset();cshev1.LP.setup(Order,filterSampleRate,filterFrequency,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {cshev1.HP.reset();cshev1.HP.setup(Order,filterSampleRate,filterFrequency,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {cshev1.LS.reset();cshev1.LS.setup(Order,filterSampleRate,filterFrequency,filterGain,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {cshev1.HS.reset();cshev1.HS.setup(Order,filterSampleRate,filterFrequency,filterGain,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {cshev1.BP.reset();cshev1.BP.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {cshev1.BS.reset();cshev1.BS.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVI) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {cshev1.SH.reset();cshev1.SH.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterGain,filterRipple);}
                /* ChebyshevII Filters */
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::LOWPASS))   {cshev2.LP.reset();cshev2.LP.setup(Order,filterSampleRate,filterFrequency,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::HIGHPASS))  {cshev2.HP.reset();cshev2.HP.setup(Order,filterSampleRate,filterFrequency,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::LOWSHELF))  {cshev2.LS.reset();cshev2.LS.setup(Order,filterSampleRate,filterFrequency,filterGain,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)) {cshev2.HS.reset();cshev2.HS.setup(Order,filterSampleRate,filterFrequency,filterGain,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::BANDPASS))  {cshev2.BP.reset();cshev2.BP.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::BANDSTOP))  {cshev2.BS.reset();cshev2.BS.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterRipple);}
                else if ((Name==DSPFILTERS::FilterNames::CHEBYSHEVII) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)) {cshev2.SH.reset();cshev2.SH.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterGain,filterRipple);}
                /* Elliptic Filters */
                else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::LOWPASS))  {ellipt.LP.reset();ellipt.LP.setup(Order,filterSampleRate,filterFrequency,filterRipple,filterRolloff);}
                else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::HIGHPASS)) {ellipt.HP.reset();ellipt.HP.setup(Order,filterSampleRate,filterFrequency,filterRipple,filterRolloff);}
                else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::BANDPASS)) {ellipt.BP.reset();ellipt.BP.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterRipple,filterRolloff);}
                else if ((Name==DSPFILTERS::FilterNames::ELLIPTIC) and (Type==DSPFILTERS::FilterTypes::BANDSTOP)) {ellipt.BS.reset();ellipt.BS.setup(Order,filterSampleRate,filterFrequency,filterWidth,filterRipple,filterRolloff);}
                /* Legendre Filters */
                else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::LOWPASS))  {legend.LP.reset();legend.LP.setup(Order,filterSampleRate,filterFrequency);}
                else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::HIGHPASS)) {legend.HP.reset();legend.HP.setup(Order,filterSampleRate,filterFrequency);}
                else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::BANDPASS)) {legend.BP.reset();legend.BP.setup(Order,filterSampleRate,filterFrequency,filterWidth);}
                else if ((Name==DSPFILTERS::FilterNames::LEGENDRE) and (Type==DSPFILTERS::FilterTypes::BANDSTOP)) {legend.BS.reset();legend.BS.setup(Order,filterSampleRate,filterFrequency,filterWidth);}
                /* Robert Bristow-Johnson Filters */
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::LOWPASS))  {rbjohn.LP.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);} /* these handle reset() on their own */
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::HIGHPASS)) {rbjohn.HP.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::LOWSHELF)) {rbjohn.LS.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::HIGHSHELF)){rbjohn.HS.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDPASS)) {rbjohn.BP1.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDPASS2)){rbjohn.BP2.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDSTOP)) {rbjohn.BS.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::BANDSHELF)){rbjohn.SH.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}
                else if ((Name==DSPFILTERS::FilterNames::COOKBOOK) and (Type==DSPFILTERS::FilterTypes::ALLPASS))  {rbjohn.AP.setup(filterSampleRate,filterFrequency,filterWidth,filterGain);}

                //  Don't forget to end the refresh once everything's set.
                //  Feel free to comment out if you like wasting shameful
                //  amounts of CPU... wanna guess how I found out? *sigh*
                coeffsUpdated=false;
            }
        }
    }; // end class Wrapper
    
// ---------------------------------------------------------------------------------------------------------------------------------------------------- //
// ------------------------------- AUDIOSAMPLEBUFFER BASED FILTERS ------------------------------------------------------------------------------------ //
// ---------------------------------------------------------------------------------------------------------------------------------------------------- //
    
    //
    //  SKELETON CLASS
    //
    //  This provides the structure for the filters you're going to use in your JUCE code.
    //
    //  It automatically instantiates and maintains the settings for as many filters as are
    //  required to process all the samples in all the channels of an AudioSampleBuffer that
    //  you send through its process() method.
    //
    template <DSPFILTERS::FilterNames Name, DSPFILTERS::FilterTypes Type, int Order>
    class Skeleton
    {
    public:
        
        Skeleton () {}
        virtual ~Skeleton () {}
        
    protected:
        
        //
        //  PARAMETER SETTER and GETTER methods
        //
        //  These are not publicly accessible, the filter wrappers
        //  will publish the ones that are fitting for the filter.
        //
        void setSampleRate (double SR) {filterSampleRate=SR;}
        void setFrequency  (double Hz) {filterFrequency=Hz;}
        void setWidth      (double Hz) {filterWidth=Hz;}
        void setGain       (double dB) {filterGain=dB;}
        void setRipple     (double dB) {filterRipple=dB;}
        void setRolloff    (double dB) {filterRolloff=dB;}
        
        double getSampleRate()  {return filterSampleRate;}
        double getFrequency()   {return filterFrequency;}
        double getWidth()       {return filterWidth;}
        double getGain()        {return filterGain;}
        double getRipple()      {return filterRipple;}
        double getRolloff()     {return filterRolloff;}
        
        //
        //  FILTER ARRAY that dynamically instantiates new or deletes old filters.
        //
        //  Holds one filter per channel to process. Its content is
        //  handled in prepareFilters() below.
        //
        OwnedArray<DSPFILTERS::Wrappers::Wrapper<Name,Type,Order>> filterArray;
        
        
        //
        //  FILTER SETTINGS
        //
        //  These properties are passed down to the individual channel
        //  filters in the setup() or process() methods.
        //
        //  They all need a default value, shouldn't be undefined.
        //
        double filterSampleRate=0.0;    // in Hz
        double filterFrequency=0.0;     // in Hz
        double filterWidth=0.0;         // in Hz
        double filterGain=0.0;          // in dB
        double filterRipple=0.0;        // in dB
        double filterRolloff=0.0;       // in dB
        
        //
        //  FILTER ALLOCATION
        //
        //  Re-/populates the "one filter for each channel" array.
        //
        //  Make sure this is called in EVERY process()! Nothing will
        //  happen if nothing has changed, but if something did change
        //  and this isn't run... well, don't go there.
        //
        void prepareFilters(int numChannels)
        {
            /* reset array if numFilters doesn't match numChannels */
            if(!filterArray.isEmpty())
            {
                if(filterArray.size()!=numChannels)
                {
                    filterArray.clear(true);
                }
            }
            
            /* if array empty then add numChannels filter instances */
            if(filterArray.isEmpty())
            {
                for(int channel=0;channel<numChannels;++channel)
                {
                    filterArray.add(new DSPFILTERS::Wrappers::Wrapper<Name,Type,Order>);
                }
            }
        }
        
        //
        //  FILTER SETUP
        //
        //  A single ginormous method that will update EVERY property of a filter.
        //
        //  Not all filters have (need for) all parameters and properties there are.
        //  The filter wrappers will publish simplified abstractions of this one,
        //  always expecting the correct set of arguments for their filter kind.
        //
        void setup (double SR, double Hz, double Width, double dB, double Ripple__or__Stopband, double Rolloff)
        {
            setSampleRate(SR);
            setFrequency(Hz);
            setWidth(Width);
            setGain(dB);
            setRipple(Ripple__or__Stopband);
            setRolloff(Rolloff);
        }
        
        //
        //  FILTER PROCESS for an entire AudioSampleBuffer in one go.
        //
        //  Uses current filter settings of Skeleton class, so make sure they're
        //  correctly set before calling this.
        //
        //  Convenient one-liner with merged setup() call below.
        //
        //  Not all filters have (need for) all parameters and properties there are.
        //  The filter wrappers will publish simplified abstractions of this one,
        //  always expecting the correct set of arguments for their filter kind.
        //
        virtual void process (AudioSampleBuffer* stream)
        {
            const int numChannels=stream->getNumChannels();
            const int numSamples=stream->getNumSamples();
            
            prepareFilters(numChannels);
            
            jassert(filterSampleRate>0.0);
            jassert(filterFrequency>0.0);
            jassert(!filterArray.isEmpty());
            jassert(filterArray.size()==numChannels);
            
            for(int channel=0;channel<numChannels;++channel)
            {
                float* channelData=stream->getWritePointer(channel);
                
                DSPFILTERS::Wrappers::Wrapper<Name,Type,Order>* channelFilter;
                channelFilter=filterArray[channel];
                
                for(int sample=0;sample<numSamples;++sample)
                {
                    // This huge command doesn't care what the wrapped filter is.
                    // It just shoves all the Skeleton filter properties into it
                    // and the wrapped filter will update itself accordingly, as
                    // well as find and use the parameters it needs by itself.
                    channelData[sample]=channelFilter->processSample(channelData[sample],
                                                                     filterSampleRate,
                                                                     filterFrequency,
                                                                     filterWidth,
                                                                     filterGain,
                                                                     filterRipple,
                                                                     filterRolloff);
                }
            }
        }
        
        //  Convenient one-liner that you'll never use, because it's protected and
        //  the filters will always offer you a fitting simplification of this.
        virtual void process (AudioSampleBuffer* stream, double SR, double Hz, double Width, double dB, double Ripple__or__Stopband, double Rolloff)
        {
            setup(SR,Hz,Width,dB,Ripple__or__Stopband,Rolloff);
            process(stream);
        }
        
    }; // end class Skeleton
    
} // end namespace Wrappers

#endif // WRAPPERS_H_INCLUDED
