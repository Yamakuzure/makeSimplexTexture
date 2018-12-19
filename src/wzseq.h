#ifndef PWX_MAKESIMPLEXTEXTURE_WZSEQ_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_WZSEQ_H_INCLUDED 1
#pragma once

/** @file wzseq.h
 *
 * This file is part of makeSimplexTexture
 *
 * (c) 2007 - 2018 PrydeWorX
 * @author Sven Eden, PrydeWorX - Bardowick, Germany
 *         sven.eden@prydeworx.com
 *         https://github.com/Yamakuzure/makeSimplexTexture ; https://prydeworx.com/makeSimplexTexture
 *
 * makeSimplexTexture is free software under MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * History and change log are maintained in main.cpp
 **/


// Note: These are just the sequence enums used by ENVIRONMENT, and
//       their respective operators.
//       The file is used to keep environment.h readable.

/** @brief enum for Z-Coordinate sequences
  *
  * This enum lists sequence patterns for the z coordinate in 3D/4D mode
**/
enum eSequenceZ {
    eiZNone = 0,//!< No sequence, Z ist static (default)
    eiZincX,    //!< Z += modZ whenever X is modified
    eiZincY,    //!< Z += modZ whenever Y is modified
    eiZincW,    //!< Z += modZ whenever W is modified
    eiZisX,     //!< Z equals X
    eiZisY,     //!< Z equals Y
    eiZisW,     //!< Z equals W
    eiZisXY,    //!< Z equals X * Y
    eiZisXYdW,  //!< Z equals (X * Y) / W
    eiZisXYmW,  //!< Z equals (X * Y) % W
    eiZisXdY,   //!< Z equals X / Y
    eiZisYdX,   //!< Z equals Y / X
    eiZisXaY,   //!< Z equals X + Y
    eiZisXsY,   //!< Z equals X - Y
    eiZisYsX,   //!< Z equals Y - X
    eiZisXmY,   //!< Z equals X % Y
    eiZisYmX,   //!< Z equals Y % Y
    eiZlast     //!< unused helper for the operators
};

/** @brief enum for W-Coordinate sequences
  *
  * This enum lists sequence patterns for the w coordinate in 4D mode
**/
enum eSequenceW {
    eiWNone = 0,//!< No sequence, W ist static (default)
    eiWincX,    //!< W += modW whenever X is modified
    eiWincY,    //!< W += modW whenever Y is modified
    eiWincZ,    //!< W += modW whenever Z is modified
    eiWisX,     //!< W equals X
    eiWisY,     //!< W equals Y
    eiWisZ,     //!< W equals Z
    eiWisXY,    //!< W equals X * Y
    eiWisXYdZ,  //!< W equals (X * Y) / Z
    eiWisXYmZ,  //!< W equals (X * Y) % Z
    eiWisXdY,   //!< W equals X / Y
    eiWisYdX,   //!< W equals Y / X
    eiWisXaY,   //!< W equals X + Y
    eiWisXsY,   //!< W equals X - Y
    eiWisYsX,   //!< W equals Y - X
    eiWisXmY,   //!< W equals X % Y
    eiWisYmX,   //!< W equals Y % Y
    eiWlast     //!< unused helper for the operators
};

// --- Operators for easier incrementation/decrementation


eSequenceZ operator++( eSequenceZ& seq, int32_t );
eSequenceZ operator--( eSequenceZ& seq, int32_t );
eSequenceW operator++( eSequenceW& seq, int32_t );
eSequenceW operator--( eSequenceW& seq, int32_t );


#endif // PWX_MAKESIMPLEXTEXTURE_WZSEQ_H_INCLUDED
