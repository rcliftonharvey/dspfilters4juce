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
//  https://github.com/rc-h/dspfilters
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

#ifndef _TEMPLATES_H_INCLUDED
#define _TEMPLATES_H_INCLUDED

/* Common.h stays global */
#ifndef DSPFILTERS_COMMON_H
#include "Shared/Common.h"
#endif

//
//  These are the modified originals, they'll only be used as templates
//  for the actual JUCE filter classes; you should normally not come in
//  contact with these; Don't use them unless you know why you want to.
//
//  Inclusion order respects dependencies. Don't mess it up, or else...
//

namespace DSPFILTERS
{
    namespace Templates
    {
/* -------- START INCLUDING IN CORRECT ORDER -------- */

        
#ifndef DSPFILTERS_UTILITIES_H
#include "Shared/Utilities.h"
#endif
        
#ifndef DSPFILTERS_MATHSUPPLEMENT_H
#include "Shared/MathSupplement.h"
#endif

#ifndef DSPFILTERS_TYPES_H
#include "Shared/Types.h"
#endif
        
#ifndef DSPFILTERS_LAYOUT_H
#include "Shared/Layout.h"
#endif
        
#ifndef DSPFILTERS_ROOTFINDER_H
#include "Shared/RootFinder.h"
#endif
        
#ifndef DSPFILTERS_BIQUAD_H
#include "Shared/Biquad.h"
#endif
        
#ifndef DSPFILTERS_RBJ_H
#include "RBJ.h"
#endif

#ifndef DSPFILTERS_CASCADE_H
#include "Shared/Cascade.h"
#endif
        
#ifndef DSPFILTERS_STATE_H
#include "Shared/State.h"
#endif
        
#ifndef DSPFILTERS_POLEFILTER_H
#include "Shared/PoleFilter.h"
#endif

#ifndef DSPFILTERS_BESSEL_H
#include "Bessel.h"
#endif

#ifndef DSPFILTERS_BUTTERWORTH_H
#include "Butterworth.h"
#endif
        
#ifndef DSPFILTERS_CHEBYSHEVI_H
#include "ChebyshevI.h"
#endif

#ifndef DSPFILTERS_CHEBYSHEVII_H
#include "ChebyshevII.h"
#endif
        
#ifndef DSPFILTERS_ELLIPTIC_H
#include "Elliptic.h"
#endif
        
#ifndef DSPFILTERS_LEGENDRE_H
#include "Legendre.h"
#endif

        
/* --------------- END OF INCLUSIONS --------------- */
    } // end namespace Templates
} // end namespace #DSPFILTERS

#endif  //  _TEMPLATES_H_INCLUDED
