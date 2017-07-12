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

#ifndef _WRAPPERS_H_INCLUDED
#define _WRAPPERS_H_INCLUDED

//
//  WRAPPER INCLUSION
//
//  This file will create a namespace for the DSPFILTERS and include all
//  module headers with filter wrappers. It also defines a few ENUMs you
/// can use within the DSPFILTERS (or altered) namespace.
//

namespace DSPFILTERS
{
//
//  Used for easier declaration.
//  They do NOT have the same indices as in the DspFilters Kind enum!
//
    enum FilterNames
    {
        BESSEL=0,
        BIQUAD,
        BUTTERWORTH,
        CHEBYSHEVI,
        CHEBYSHEVII,
        CUSTOM,
        ELLIPTIC,
        LEGENDRE,
        COOKBOOK /* to avoid RBJ redefinition */
    };
    
    enum FilterTypes
    {
        LOWPASS=0,
        HIGHPASS,
        ALLPASS,
        BANDPASS,
        BANDPASS2,
        BANDSTOP,
        LOWSHELF,
        HIGHSHELF,
        BANDSHELF
    };
  
/* ---------------- START INCLUDING ---------------- */
    
//
//  This is not a real wrapper class, it just adds a fake "order" to
//  RBJ Biquads that they otherwise wouldn't have.
//
#ifndef WRAPPER_RBJARRAY_H_INCLUDED
#include "RBJArray.h"
#endif
    
//
//  This is the file that contains the definitions for the two major
//  wrapper classes. DSPFilters to single channel abstract and single
//  channel abstract to AudioSampleBuffer processor.
//
#ifndef WRAPPER_H_INCLUDED
#include "Wrappers.h"
#endif

//
//  These are the JUCE adapted classes.
//  Use these classes in your projects.
//
#ifndef WRAPPER_BESSEL_H_INCLUDED
#include "Bessel.h"
#endif
    
#ifndef WRAPPER_BUTTERWORTH_H_INCLUDED
#include "Butterworth.h"
#endif
    
#ifndef WRAPPER_CHEBYSHEVI_H_INCLUDED
#include "ChebyshevI.h"
#endif
    
#ifndef WRAPPER_CHEBYSHEVII_H_INCLUDED
#include "ChebyshevII.h"
#endif
    
#ifndef WRAPPER_ELLIPTIC_H_INCLUDED
#include "Elliptic.h"
#endif
    
#ifndef WRAPPER_LEGENDRE_H_INCLUDED
#include "Legendre.h"
#endif
    
#ifndef WRAPPER_RBJ_H_INCLUDED
#include "RBJ.h"
#endif

        
/* --------------- END OF INCLUSIONS --------------- */
} // end namespace DSPFILTERS

// Clean up
#ifdef WRAPPER
#undef WRAPPER
#endif

#ifdef SKELETON
#undef SKELETON
#endif

#endif  //  _WRAPPERS_H_INCLUDED
