/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco and adapted by Bernd Porr for Linux

Official project location:
https://github.com/vinniefalco/DSPFilters

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vinnie Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#ifndef DSPFILTERS_BUTTERWORTH_H
#define DSPFILTERS_BUTTERWORTH_H

/*
 * Filters with Butterworth response characteristics
 *
 */

namespace Butterworth
{
    // Half-band analog prototypes (s-plane)
    class AnalogLowPass : public LayoutBase
    {
    public:
        
        AnalogLowPass () : m_numPoles (-1)
        {
            setNormal (0, 1);
        }
        
        void design (const int numPoles)
        {
            if (m_numPoles != numPoles)
            {
                m_numPoles = numPoles;
                
                reset ();
                
                const double n2 = 2 * numPoles;
                const int pairs = numPoles / 2;
                for (int i = 0; i < pairs; ++i)
                {
                    complex_t c = std::polar (1., doublePi_2 + (2 * i + 1) * doublePi / n2);
                    addPoleZeroConjugatePairs (c, infinity());
                }
                
                if (numPoles & 1)
                    add (-1, infinity());
            }
        }
        
    private:
        
        int m_numPoles;
        
    };
    
//------------------------------------------------------------------------------
    
    class AnalogLowShelf : public LayoutBase
    {
    public:
        
        AnalogLowShelf () : m_numPoles (-1)
        {
            setNormal (doublePi, 1);
        }
        
        void design (int numPoles, double gainDb)
        {
            if (m_numPoles != numPoles ||
                m_gainDb != gainDb)
            {
                m_numPoles = numPoles;
                m_gainDb = gainDb;
                
                reset ();
                
                const double n2 = numPoles * 2;
                const double g = pow (pow (10., gainDb/20), 1. / n2);
                const double gp = -1. / g;
                const double gz = -g;
                
                const int pairs = numPoles / 2;
                for (int i = 1; i <= pairs; ++i)
                {
                    const double theta = doublePi * (0.5 - (2 * i - 1) / n2);
                    addPoleZeroConjugatePairs (std::polar (gp, theta), std::polar (gz, theta));
                }
                
                if (numPoles & 1)
                    add (gp, gz);
            }
        }
        
    private:
        
        int m_numPoles;
        double m_gainDb;
    };
    
//------------------------------------------------------------------------------
    
    // Factored implementations to reduce template instantiations
    
    struct LowPassBase : PoleFilterBase <AnalogLowPass>
    {
        void setup (int order, double sampleRate, double cutoffFrequency)
        {
            m_analogProto.design (order);
            
            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
    struct HighPassBase : PoleFilterBase <AnalogLowPass>
    {
        void setup (int order, double sampleRate, double cutoffFrequency)
        {
            m_analogProto.design (order);
            
            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
    struct BandPassBase : PoleFilterBase <AnalogLowPass>
    {
        void setup (int order, double sampleRate, double centerFrequency, double widthFrequency)
        {
            m_analogProto.design (order);
            
            BandPassTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
    struct BandStopBase : PoleFilterBase <AnalogLowPass>
    {
        void setup (int order, double sampleRate, double centerFrequency, double widthFrequency)
        {
            m_analogProto.design (order);
            
            BandStopTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
    struct LowShelfBase : PoleFilterBase <AnalogLowShelf>
    {
        void setup (int order, double sampleRate, double cutoffFrequency, double gainDb)
        {
            m_analogProto.design (order, gainDb);
            
            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
    struct HighShelfBase : PoleFilterBase <AnalogLowShelf>
    {
        void setup (int order, double sampleRate, double cutoffFrequency, double gainDb)
        {
            m_analogProto.design (order, gainDb);
            
            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
    struct BandShelfBase : PoleFilterBase <AnalogLowShelf>
    {
        void setup (int order, double sampleRate, double centerFrequency,
                    double widthFrequency, double gainDb)
        {
            m_analogProto.design (order, gainDb);
            
            BandPassTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);
            
            // HACK!
            m_digitalProto.setNormal (((centerFrequency/sampleRate) < 0.25) ? doublePi : 0, 1);
            
            Cascade::setLayout (m_digitalProto);
        }
    };
    
//------------------------------------------------------------------------------
    
    //
    // Raw filters
    //
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct LowPass : PoleFilter <LowPassBase, StateType, MaxOrder>
    {
    };
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct HighPass : PoleFilter <HighPassBase, StateType, MaxOrder>
    {
    };
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct BandPass : PoleFilter <BandPassBase, StateType, MaxOrder, MaxOrder*2>
    {
    };
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct BandStop : PoleFilter <BandStopBase, StateType, MaxOrder, MaxOrder*2>
    {
    };
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct LowShelf : PoleFilter <LowShelfBase, StateType, MaxOrder>
    {
    };
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct HighShelf : PoleFilter <HighShelfBase, StateType, MaxOrder>
    {
    };
    
    template <int MaxOrder, class StateType = DEFAULT_STATE>
    struct BandShelf : PoleFilter <BandShelfBase, StateType, MaxOrder, MaxOrder*2>
    {
    };
    
}

#endif
