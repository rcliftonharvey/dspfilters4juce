/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco and adapted for Linux by Bernd Porr

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

#ifndef DSPFILTERS_CASCADE_H
#define DSPFILTERS_CASCADE_H

/*
 * Holds coefficients for a cascade of second order sections.
 */

// Factored implementation to reduce template instantiations
class Cascade
{
    
public:
    
    struct Stage : Biquad
    {
    };
    
    struct Storage
    {
        Storage (int maxStages_, Stage* stageArray_)
        : maxStages (maxStages_)
        , stageArray (stageArray_)
        {
        }
        
        int maxStages;
        Stage* stageArray;
    };
    
    int getNumStages () const
    {
        return m_numStages;
    }
    
    const Stage& operator[] (int index)
    {
        assert (index >= 0 && index <= m_numStages);
        return m_stageArray[index];
    }
    
    // Calculate filter response at the given normalized frequency.
    complex_t response (double normalizedFrequency) const
    {
        double w = 2 * doublePi * normalizedFrequency;
        const complex_t czn1 = std::polar (1., -w);
        const complex_t czn2 = std::polar (1., -2 * w);
        complex_t ch (1);
        complex_t cbot (1);
        
        const Biquad* stage = m_stageArray;
        for (int i = m_numStages; --i >=0; ++stage)
        {
            complex_t cb (1);
            complex_t ct    (stage->getB0()/stage->getA0());
            ct = addmul (ct, stage->getB1()/stage->getA0(), czn1);
            ct = addmul (ct, stage->getB2()/stage->getA0(), czn2);
            cb = addmul (cb, stage->getA1()/stage->getA0(), czn1);
            cb = addmul (cb, stage->getA2()/stage->getA0(), czn2);
            ch   *= ct;
            cbot *= cb;
        }
        
        return ch / cbot;
    }
    
    std::vector<PoleZeroPair> getPoleZeros () const
    {
        std::vector<PoleZeroPair> vpz;
        vpz.reserve (m_numStages);
        
        const Stage* stage = m_stageArray;
        for (int i = m_numStages; --i >=0;)
        {
            BiquadPoleState bps (*stage++);
            assert (!bps.isSinglePole() || i == 0);
            vpz.push_back (bps);
        }
        
        return vpz;
    }
    
protected:
    
    Cascade ()  : m_numStages(0),m_maxStages(0),m_stageArray(0) {}
    
    void setCascadeStorage (const Storage& storage)
    {
        m_numStages = 0;
        m_maxStages = storage.maxStages;
        m_stageArray = storage.stageArray;
    }
    
    void applyScale (double scale)
    {
        // For higher order filters it might be helpful
        // to spread this factor between all the stages.
        assert (m_numStages > 0);
        m_stageArray->applyScale (scale);
    }
    
    void setLayout (const LayoutBase& proto)
    {
        const int numPoles = proto.getNumPoles();
        m_numStages = (numPoles + 1)/ 2;
        assert (m_numStages <= m_maxStages);
        
        Biquad* stage = m_stageArray;
        for (int i = 0; i < m_numStages; ++i, ++stage)
            stage->setPoleZeroPair (proto[i]);
        
        applyScale (proto.getNormalGain() /
                    std::abs (response (proto.getNormalW() / (2 * doublePi))));
    }
    
private:
    int m_numStages;
    int m_maxStages;
    Stage* m_stageArray;
};

//------------------------------------------------------------------------------

// Storage for Cascade
template <int MaxStages,class StateType>
class CascadeStages
{
public:
    void reset ()
    {
        StateType* state = m_states;
        for (int i = MaxStages; --i >= 0; ++state)
            state->reset();
    }
    
public:
    template <typename Sample>
    inline Sample filter(const Sample in)
    {
        double out = in;
        StateType* state = m_states;
        Biquad const* stage = m_stages;
        for (int i = MaxStages; --i >= 0; ++state, ++stage)
            out = state->process1 (out, *stage);
        return static_cast<Sample> (out);
    }
    
    Cascade::Storage getCascadeStorage()
    {
        return Cascade::Storage (MaxStages, m_stages);
    }
    
private:
    Cascade::Stage m_stages[MaxStages];
    StateType m_states[MaxStages];
};

#endif
