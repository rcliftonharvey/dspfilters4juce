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

#ifndef DSPFILTERS_H_INCLUDED
#define DSPFILTERS_H_INCLUDED

//
//  DSPFilters4JUCE
//
//  Add all files in the DSPFilters4JUCE folder to your JUCE project and then
//  #include this DSPFilters.h header in the PluginProcessor.h file.
//
//  That's all.
//


//  Change the value of the following line if you wish to have the filters inside
//  another namespace. Whenever you see DSPFILTERS written in example code, this
//  is what you have to substitute it with.
#define DSPFILTERS DSP


//  This loads the DSPFilters library that Bernd Porr adapted for Linux. No, it's
//  fine to be used on macOS and Windows.
#include "Templates/_Templates.h"


//  This loads my "wrapper" classes for easy use of DSPFilters in the JUCE.
#include "Wrappers/_Wrappers.h"


//  The namespace has already been applied to all included modules. If you really
//  want, you can comment out these 3 next lines, leaving the token DSPFILTERS as
//  an additional namespace. Not necessary, but it's here in case you need it.
#ifdef DSPFILTERS
#undef DSPFILTERS
#endif


#endif  // DSPFILTERS_H_INCLUDED
