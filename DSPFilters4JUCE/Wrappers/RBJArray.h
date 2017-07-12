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
//  https://github.com/rcliftonharvey/dspfilters4juce
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

#ifndef WRAPPER_RBJARRAY_H_INCLUDED
#define WRAPPER_RBJARRAY_H_INCLUDED

namespace Templates
{
    //
    //  Helper class that loads an array of raw RBJ Biquad filters and
    //  processes them in series to create a fake "order" like all the
    //  other filters in the DSPFilters library have.
    //
    template<DSPFILTERS::FilterTypes Type, unsigned int Order=1>
    class RBJArray
    {
    public:
        
        //
        //  UPDATES the SETTINGS for all filters in this RBJArray.
        //
        //  If the passed values are the same as the filters' current settings,
        //  nothing will happen.
        //
        //  Only if any of the passed values vary from the filters' current
        //  settings, the coeffsUpdated flag will be set. In the next process()
        //  if this flag is TRUE, filters will be reset and their values updated.
        //
        void setup (double SR, double Hz, double Q, double dB)
        {
            if(SR!=filterSampleRate){filterSampleRate=SR;coeffsUpdated=true;}
            if(Hz!=filterFrequency){filterFrequency=Hz;coeffsUpdated=true;}
            if(Q!=filterWidth){filterWidth=Q;coeffsUpdated=true;}
            if(dB!=filterGain){filterGain=dB;coeffsUpdated=true;}
        }
        
        //
        //  The actual PER SAMPLE processing for an array of RBJs.
        //
        //  Note that I named this .filter() instead of .process() or .processSample()
        //
        //  I did this to make the RBJArray helper class conform and fit in among the
        //  DSPFilters that Bernd Porr simplified. In one of his steps, most of the
        //  .process() methods evolved into .filter() methods.
        //
        double filter (double input)
        {
            double filtered=input;
            
            prepareFilters();
            updateCoefficients();
            
            jassert(filterSampleRate>0.0);
            jassert(filterFrequency>0.0);
            jassert(filterState.isEmpty()==false);
            jassert(filterState.size()==Order);

            if      (Type==DSPFILTERS::FilterTypes::LOWPASS)
            {
                jassert(LPs.isEmpty()==false);
                jassert(LPs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=LPs[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::HIGHPASS)
            {
                jassert(HPs.isEmpty()==false);
                jassert(HPs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=HPs[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::LOWSHELF)
            {
                jassert(LSs.isEmpty()==false);
                jassert(LSs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=LSs[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::HIGHSHELF)
            {
                jassert(HSs.isEmpty()==false);
                jassert(HSs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=HSs[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDPASS)
            {
                jassert(BP1s.isEmpty()==false);
                jassert(BP1s.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=BP1s[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDPASS2)
            {
                jassert(BP2s.isEmpty()==false);
                jassert(BP2s.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=BP2s[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDSTOP)
            {
                jassert(BSs.isEmpty()==false);
                jassert(BSs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=BSs[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDSHELF)
            {
                jassert(SHs.isEmpty()==false);
                jassert(SHs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=SHs[step]->filter(filtered,*filterState[step]);
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::ALLPASS)
            {
                jassert(APs.isEmpty()==false);
                jassert(APs.size()==Order);
                
                for(unsigned int step=0;step<Order;++step)
                {
                    filtered=APs[step]->filter(filtered,*filterState[step]);
                }
            }
            
            return filtered;
        }
        
    private:
        
        bool coeffsUpdated=false;
        
        //
        //  FILTER SETTINGS
        //
        //  These values are used by all the RBJ filters in the array.
        //
        //  They all need a default value, shouldn't be undefined.
        //
        double filterSampleRate=0.0;    // in Hz
        double filterFrequency=0.0;     // in Hz
        double filterWidth=0.5;         // guessing Q factor?
        double filterGain=0.0;          // in dB
        
        //
        //  FILTER STATE
        //
        //  This array holds one State per "order" filter.
        //
        OwnedArray<DSPFILTERS::Templates::DEFAULT_STATE> filterState;// filter states buffer
        
        //
        //  FILTER ARRAYS for every RBJ filter type.
        //
        //  These hold one filter per "order" to process.
        //  Their content is handled in prepareFilters() below.
        //
        //  Only the array for the current filter type is ever
        //  populated with filters, so no worry about resources.
        //
        OwnedArray<DSPFILTERS::Templates::RBJ::LowPass>   LPs;
        OwnedArray<DSPFILTERS::Templates::RBJ::HighPass>  HPs;
        OwnedArray<DSPFILTERS::Templates::RBJ::LowShelf>  LSs;
        OwnedArray<DSPFILTERS::Templates::RBJ::HighShelf> HSs;
        OwnedArray<DSPFILTERS::Templates::RBJ::BandPass1> BP1s;
        OwnedArray<DSPFILTERS::Templates::RBJ::BandPass2> BP2s;
        OwnedArray<DSPFILTERS::Templates::RBJ::BandStop>  BSs;
        OwnedArray<DSPFILTERS::Templates::RBJ::BandShelf> SHs;
        OwnedArray<DSPFILTERS::Templates::RBJ::AllPass>   APs;
        
        //
        //  FILTER ALLOCATION
        //
        //  Re-/populates the "one filter for each channel" array.
        //
        //  Make sure this is called in EVERY process()! Nothing will
        //  happen if nothing has changed, but if something did change
        //  and this isn't run... well, don't go there.
        //
        void prepareFilters ()
        {
            if      (Type==DSPFILTERS::FilterTypes::LOWPASS)
            {
                if(!LPs.isEmpty())
                {
                    if(LPs.size()!=Order)
                    {
                        LPs.clear(true);
                    }
                }
                
                if(LPs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        LPs.add(new DSPFILTERS::Templates::RBJ::LowPass);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::HIGHPASS)
            {
                if(!HPs.isEmpty())
                {
                    if(HPs.size()!=Order)
                    {
                        HPs.clear(true);
                    }
                }
                
                if(HPs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        HPs.add(new DSPFILTERS::Templates::RBJ::HighPass);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::LOWSHELF)
            {
                if(!LSs.isEmpty())
                {
                    if(LSs.size()!=Order)
                    {
                        LSs.clear(true);
                    }
                }
                
                if(LSs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        LSs.add(new DSPFILTERS::Templates::RBJ::LowShelf);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::HIGHSHELF)
            {
                if(!HSs.isEmpty())
                {
                    if(HSs.size()!=Order)
                    {
                        HSs.clear(true);
                    }
                }
                
                if(HSs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        HSs.add(new DSPFILTERS::Templates::RBJ::HighShelf);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDPASS)
            {
                if(!BP1s.isEmpty())
                {
                    if(BP1s.size()!=Order)
                    {
                        BP1s.clear(true);
                    }
                }
                
                if(BP1s.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        BP1s.add(new DSPFILTERS::Templates::RBJ::BandPass1);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDPASS2)
            {
                if(!BP2s.isEmpty())
                {
                    if(BP2s.size()!=Order){BP2s.clear(true);
                    }
                }
                
                if(BP2s.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        BP2s.add(new DSPFILTERS::Templates::RBJ::BandPass2);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDSTOP)
            {
                if(!BSs.isEmpty())
                {
                    if(BSs.size()!=Order)
                    {
                        BSs.clear(true);
                    }
                }
                
                if(BSs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        BSs.add(new DSPFILTERS::Templates::RBJ::BandStop);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::BANDSHELF)
            {
                if(!SHs.isEmpty())
                {
                    if(SHs.size()!=Order)
                    {
                        SHs.clear(true);
                    }
                }
                
                if(SHs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        SHs.add(new DSPFILTERS::Templates::RBJ::BandShelf);
                    }
                }
            }
            else if (Type==DSPFILTERS::FilterTypes::ALLPASS)
            {
                if(!APs.isEmpty())
                {
                    if(APs.size()!=Order)
                    {
                        APs.clear(true);
                    }
                }
                
                if(APs.isEmpty())
                {
                    for(unsigned int step=0;step<Order;++step)
                    {
                        APs.add(new DSPFILTERS::Templates::RBJ::AllPass);
                    }
                }
            }
        }
        
        
        //
        //  If any settings have changed, re-calculate these filters' settings.
        //  If no settings have changed, nothing happens.
        //
        //  Make sure this is run EVERY processSample() BEFORE the filters
        //  actually process something, otherwise the filters won't know what
        //  to do, or possibly do the wrong thing. Which is never a good idea.
        //
        void updateCoefficients ()
        {
            if (coeffsUpdated==true)
            {
                /* reset filter states */
                filterState.clear(true);

                for(int step=0;step<Order;++step)
                {
                    filterState.add(new DSPFILTERS::Templates::DEFAULT_STATE);
                }
                
                
                if      (Type==DSPFILTERS::FilterTypes::LOWPASS)
                {
                    if(!LPs.isEmpty())
                    {
                        for(int filter=0;filter<LPs.size();++filter)
                        {
                            LPs[filter]->setup(filterSampleRate,filterFrequency,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::HIGHPASS)
                {
                    if(!HPs.isEmpty())
                    {
                        for(int filter=0;filter<HPs.size();++filter)
                        {
                            HPs[filter]->setup(filterSampleRate,filterFrequency,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::LOWSHELF)
                {
                    if(!LSs.isEmpty())
                    {
                        for(int filter=0;filter<LSs.size();++filter)
                        {
                            LSs[filter]->setup(filterSampleRate,filterFrequency,filterGain,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::HIGHSHELF)
                {
                    if(!HSs.isEmpty())
                    {
                        for(int filter=0;filter<HSs.size();++filter)
                        {
                            HSs[filter]->setup(filterSampleRate,filterFrequency,filterGain,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::BANDPASS)
                {
                    if(!BP1s.isEmpty())
                    {
                        for(int filter=0;filter<BP1s.size();++filter)
                        {
                            BP1s[filter]->setup(filterSampleRate,filterFrequency,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::BANDPASS2)
                {
                    if(!BP2s.isEmpty())
                    {
                        for(int filter=0;filter<BP2s.size();++filter)
                        {
                            BP2s[filter]->setup(filterSampleRate,filterFrequency,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::BANDSTOP)
                {
                    if(!BSs.isEmpty())
                    {
                        for(int filter=0;filter<BSs.size();++filter)
                        {
                            BSs[filter]->setup(filterSampleRate,filterFrequency,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::BANDSHELF)
                {
                    if(!SHs.isEmpty())
                    {
                        for(int filter=0;filter<SHs.size();++filter)
                        {
                            SHs[filter]->setup(filterSampleRate,filterFrequency,filterGain,filterWidth);
                        }
                    }
                }
                else if (Type==DSPFILTERS::FilterTypes::ALLPASS)
                {
                    if(!APs.isEmpty())
                    {
                        for(int filter=0;filter<APs.size();++filter)
                        {
                            APs[filter]->setup(filterSampleRate,filterFrequency,filterWidth);
                        }
                    }
                }

                coeffsUpdated=false;
            }
        }
        
    }; // end class RBJArray
    
} // end namespace Templates

#endif // WRAPPER_RBJARRAY_H_INCLUDED
