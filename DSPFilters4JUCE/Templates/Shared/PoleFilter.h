/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco adapted by Bernd Porr for Linux

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

#ifndef DSPFILTERS_POLEFILTER_H
#define DSPFILTERS_POLEFILTER_H

/*
 * Base for filters designed via algorithmic placement of poles and zeros.
 *
 * Typically, the filter is first designed as a half-band low pass or
 * low shelf analog filter (s-plane). Then, using a transformation such
 * as the ones from Constantinides, the poles and zeros of the analog filter
 * are calculated in the z-plane.
 *
 */

// Factored implementations to reduce template instantiations
class PoleFilterBase2 : public Cascade
{
public:
    // This gets the poles/zeros directly from the digital
    // prototype. It is used to double check the correctness
    // of the recovery of pole/zeros from biquad coefficients.
    //
    // It can also be used to accelerate the interpolation
    // of pole/zeros for parameter modulation, since a pole
    // filter already has them calculated
    
#if 1
    // Commenting this out will pass the call to the Cascade,
    // which tries to compute the poles and zeros from the biquad
    // coefficients.
    std::vector<PoleZeroPair> getPoleZeros () const
    {
        std::vector<PoleZeroPair> vpz;
        const int pairs = (m_digitalProto.getNumPoles () + 1) / 2;
        for (int i = 0; i < pairs; ++i)
            vpz.push_back (m_digitalProto[i]);
        return vpz;
    }
#endif
    
protected:
    LayoutBase m_digitalProto;
};

// Serves a container to hold the analog prototype
// and the digital pole/zero layout.
template <class AnalogPrototype>
class PoleFilterBase : public PoleFilterBase2
{
protected:
    void setPrototypeStorage (const LayoutBase& analogStorage,
                              const LayoutBase& digitalStorage)
    {
        m_analogProto.setStorage (analogStorage);
        m_digitalProto = digitalStorage;
    }
    
protected:
    AnalogPrototype m_analogProto;
};

//------------------------------------------------------------------------------

// Storage for pole filters
template <class BaseClass,
class StateType,
int MaxAnalogPoles,
int MaxDigitalPoles = MaxAnalogPoles>
struct PoleFilter : BaseClass
, CascadeStages <(MaxDigitalPoles + 1) / 2 , StateType>
{
    PoleFilter ()
    {
        // This glues together the factored base classes
        // with the templatized storage classes.
        BaseClass::setCascadeStorage (this->getCascadeStorage());
        BaseClass::setPrototypeStorage (m_analogStorage, m_digitalStorage);
    }
    
private:
    Layout <MaxAnalogPoles> m_analogStorage;
    Layout <MaxDigitalPoles> m_digitalStorage;
};

//------------------------------------------------------------------------------

/*
 * s-plane to z-plane transforms
 *
 * For pole filters, an analog prototype is created via placement of
 * poles and zeros in the s-plane. The analog prototype is either
 * a halfband low pass or a halfband low shelf. The poles, zeros,
 * and normalization parameters are transformed into the z-plane
 * using variants of the bilinear transformation.
 *
 */

// low pass to low pass
class LowPassTransform
{
public:
    LowPassTransform (double fc, LayoutBase& digital, LayoutBase const& analog)
    {
        digital.reset ();
        
        // prewarp
        f = tan (doublePi * fc);
        
        const int numPoles = analog.getNumPoles ();
        const int pairs = numPoles / 2;
        for (int i = 0; i < pairs; ++i)
        {
            const PoleZeroPair& pair = analog[i];
            digital.addPoleZeroConjugatePairs (transform (pair.poles.first),
                                               transform (pair.zeros.first));
        }
        
        if (numPoles & 1)
        {
            const PoleZeroPair& pair = analog[pairs];
            digital.add (transform (pair.poles.first),
                         transform (pair.zeros.first));
        }
        
        digital.setNormal (analog.getNormalW(),
                           analog.getNormalGain());
    }
    
private:
    
    complex_t transform (complex_t c)
    {
        if (c == infinity())
            return complex_t (-1, 0);
        
        // frequency transform
        c = f * c;
        
        // bilinear low pass transform
        return (1. + c) / (1. - c);
    }
    
    double f;
};

//------------------------------------------------------------------------------

// low pass to high pass
class HighPassTransform
{
public:
    
    HighPassTransform (double fc, LayoutBase& digital, LayoutBase const& analog)
    {
        digital.reset ();
        
        // prewarp
        f = 1. / tan (doublePi * fc);
        
        const int numPoles = analog.getNumPoles ();
        const int pairs = numPoles / 2;
        for (int i = 0; i < pairs; ++i)
        {
            const PoleZeroPair& pair = analog[i];
            digital.addPoleZeroConjugatePairs (transform (pair.poles.first),
                                               transform (pair.zeros.first));
        }
        
        if (numPoles & 1)
        {
            const PoleZeroPair& pair = analog[pairs];
            digital.add (transform (pair.poles.first),
                         transform (pair.zeros.first));
        }
        
        digital.setNormal (doublePi - analog.getNormalW(),
                           analog.getNormalGain());
    }
    
private:
    
    complex_t transform (complex_t c)
    {
        if (c == infinity())
            return complex_t (1, 0);
        
        // frequency transform
        c = f * c;
        
        // bilinear high pass transform
        return - (1. + c) / (1. - c);
    }
    
    double f;
};

//------------------------------------------------------------------------------

// low pass to band pass transform
class BandPassTransform
{
    
public:
    
    BandPassTransform (double fc, double fw, LayoutBase& digital, LayoutBase const& analog)
    {
        // handle degenerate cases efficiently
        // THIS DOESNT WORK because the cascade states won't match
#if 0
        const double fw_2 = fw / 2;
        if (fc - fw_2 < 0)
        {
            LowPassTransform::transform (fc + fw_2, digital, analog);
        }
        else if (fc + fw_2 >= 0.5)
        {
            HighPassTransform::transform (fc - fw_2, digital, analog);
        }
        else
#endif
            digital.reset ();
        
        const double ww = 2 * doublePi * fw;
        
        // pre-calcs
        wc2 = 2 * doublePi * fc - (ww / 2);
        wc  = wc2 + ww;
        
        // what is this crap?
        if (wc2 < 1e-8)
            wc2 = 1e-8;
        if (wc  > doublePi-1e-8)
            wc  = doublePi-1e-8;
        
        a =     cos ((wc + wc2) * 0.5) /
        cos ((wc - wc2) * 0.5);
        b = 1 / tan ((wc - wc2) * 0.5);
        a2 = a * a;
        b2 = b * b;
        ab = a * b;
        ab_2 = 2 * ab;
        
        const int numPoles = analog.getNumPoles ();
        const int pairs = numPoles / 2;
        for (int i = 0; i < pairs; ++i)
        {
            const PoleZeroPair& pair = analog[i];
            ComplexPair p1 = transform (pair.poles.first);
            ComplexPair z1 = transform (pair.zeros.first);
            
            //
            // Optimize out the calculations for conjugates for Release builds
            //
#ifndef NDEBUG
            ComplexPair p2 = transform (pair.poles.second);
            ComplexPair z2 = transform (pair.zeros.second);
            z2=z2; /* hack to avoid "unused variable" compiler warning */
            assert (p2.first == std::conj (p1.first));
            assert (p2.second == std::conj (p1.second));
#endif
            
            digital.addPoleZeroConjugatePairs (p1.first, z1.first);
            digital.addPoleZeroConjugatePairs (p1.second, z1.second);
        }
        
        if (numPoles & 1)
        {
            ComplexPair poles = transform (analog[pairs].poles.first);
            ComplexPair zeros = transform (analog[pairs].zeros.first);
            
            digital.add (poles, zeros);
        }
        
        double wn = analog.getNormalW();
        digital.setNormal (2 * atan (sqrt (tan ((wc + wn)* 0.5) * tan((wc2 + wn)* 0.5))),
                           analog.getNormalGain());
    }
    
private:
    
    ComplexPair transform (complex_t c)
    {
        if (c == infinity())
            return ComplexPair (-1, 1);
        
        c = (1. + c) / (1. - c); // bilinear
        
        complex_t v = 0;
        v = addmul (v, 4 * (b2 * (a2 - 1) + 1), c);
        v += 8 * (b2 * (a2 - 1) - 1);
        v *= c;
        v += 4 * (b2 * (a2 - 1) + 1);
        v = std::sqrt (v);
        
        complex_t u = -v;
        u = addmul (u, ab_2, c);
        u += ab_2;
        
        v = addmul (v, ab_2, c);
        v += ab_2;
        
        complex_t d = 0;
        d = addmul (d, 2 * (b - 1), c) + 2 * (1 + b);
        
        return ComplexPair (u/d, v/d);
    }
    
    double wc;
    double wc2;
    double a;
    double b;
    double a2;
    double b2;
    double ab;
    double ab_2;
};

//------------------------------------------------------------------------------

// low pass to band stop transform
class BandStopTransform
{
public:
    
    BandStopTransform (double fc, double fw, LayoutBase& digital, LayoutBase const& analog)
    {
        digital.reset ();
        
        const double ww = 2 * doublePi * fw;
        
        wc2 = 2 * doublePi * fc - (ww / 2);
        wc  = wc2 + ww;
        
        // this is crap
        if (wc2 < 1e-8)
            wc2 = 1e-8;
        if (wc  > doublePi-1e-8)
            wc  = doublePi-1e-8;
        
        a = cos ((wc + wc2) * .5) /
        cos ((wc - wc2) * .5);
        b = tan ((wc - wc2) * .5);
        a2 = a * a;
        b2 = b * b;
        
        const int numPoles = analog.getNumPoles ();
        const int pairs = numPoles / 2;
        for (int i = 0; i < pairs; ++i)
        {
            const PoleZeroPair& pair = analog[i];
            ComplexPair p  = transform (pair.poles.first);
            ComplexPair z  = transform (pair.zeros.first);
            
            //
            // Optimize out the calculations for conjugates for Release builds
            //
#ifdef NDEBUG
            // trick to get the conjugate
            if (z.second == z.first)
                z.second = std::conj (z.first);
            
#else
            // Do the full calculation to verify correctness
            ComplexPair pc = transform (analog[i].poles.second);
            ComplexPair zc = transform (analog[i].zeros.second);
            
            // get the conjugates into pc and zc
            if (zc.first == z.first)
                std::swap (zc.first, zc.second);
            
            assert (pc.first  == std::conj (p.first));
            assert (pc.second == std::conj (p.second));
            assert (zc.first  == std::conj (z.first));
            assert (zc.second == std::conj (z.second));
            
#endif
            
            digital.addPoleZeroConjugatePairs (p.first, z.first);
            digital.addPoleZeroConjugatePairs (p.second, z.second);
        }
        
        if (numPoles & 1)
        {
            ComplexPair poles = transform (analog[pairs].poles.first);
            ComplexPair zeros = transform (analog[pairs].zeros.first);
            
            digital.add (poles, zeros);
        }
        
        if (fc < 0.25)
            digital.setNormal (doublePi, analog.getNormalGain());
        else
            digital.setNormal (0, analog.getNormalGain());
    }
    
private:
    
    ComplexPair transform (complex_t c)
    {
        if (c == infinity())
            c = -1;
        else
            c = (1. + c) / (1. - c); // bilinear
        
        complex_t u (0);
        u = addmul (u, 4 * (b2 + a2 - 1), c);
        u += 8 * (b2 - a2 + 1);
        u *= c;
        u += 4 * (a2 + b2 - 1);
        u = std::sqrt (u);
        
        complex_t v = u * -.5;
        v += a;
        v = addmul (v, -a, c);
        
        u *= .5;
        u += a;
        u = addmul (u, -a, c);
        
        complex_t d (b + 1);
        d = addmul (d, b-1, c);
        
        return ComplexPair (u/d, v/d);
    }
    
    double wc;
    double wc2;
    double a;
    double b;
    double a2;
    double b2;
};

#endif
