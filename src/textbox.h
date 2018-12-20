#ifndef PWX_MAKESIMPLEXTEXTURE_TEXTBOX_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_TEXTBOX_H_INCLUDED 1
#pragma once

/** @file textbox.h
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


struct ENVIRONMENT; // Forward declaration

#include "main.h"

// Font settings:
#if defined(_WIN32) || defined(_WIN64)
#  if !defined(FONT_PATH)
#    define FONT_PATH "C:\\WINDOWS\\Fonts"
#  endif // font path
#  if !defined(FONT_NAME)
#    define FONT_NAME "COUR.TTF"
#  endif // font
#  define FONT_SEP "\\"
#else
#  if !defined(FONT_PATH)
#    define FONT_PATH "/usr/share/fonts/freefont"
#  endif // font path
#  if !defined(FONT_NAME)
#    define FONT_NAME "FreeMono.ttf"
#  endif // font
#  define FONT_SEP "/"
#endif // Windows versus linux

// We need access to sfmlui:
#include "sfmlui.h"

/** @brief CTextBox
  *
  * A little base class for text boxes
  *
**/
class CTextBox {
  public:
    /** Default constructor **/
    explicit CTextBox();
    CTextBox( CTextBox const& )  = delete;
    CTextBox( CTextBox const&& ) = delete;
    CTextBox& operator=( CTextBox const&& ) = delete;

    /** Default destructor **/
    virtual ~CTextBox();

    /** Initializing method : **/
    virtual int32_t initialize( ENVIRONMENT* env ) PWX_VIRTUAL_PURE;

    /** struct-simulating public members **/
    sf::Color     bgColor;    //!< Background color of the text
    sf::Color     txtColor;   //!< Foreground color of the text
    sf::Image*    txtBox;     //!< The background box to draw upon
    int32_t       txtSize;    //!< Number of lines generated
    sf::Text*     txtContent; //!< dynamic array of the text lines

  private:
    /* --- no copying allowed --- */
    CTextBox( CTextBox& );
    CTextBox& operator=( CTextBox& );
};

/** @brief CHelpText
  *
  * A little simple class that holds the help text for on screen display.
  * Instanciated by showOSHelp only, thus never if not used.
  *
**/
class CHelpText: public CTextBox {
  public:
    /** Initializing method : **/
    int32_t initialize( ENVIRONMENT* env );
    /** Generate help text : **/
    int32_t generate( ENVIRONMENT* env );
};

/** @brief CStatsText
  *
  * A little simple class that holds the stats text for on screen display.
  * Instanciated by showOSStats only, thus never if not used.
  * However, to correctly display the stats, generate() has to be called!
**/
class CStatsText: public CTextBox {
  public:
    /** Initializing method : **/
    int32_t initialize( ENVIRONMENT* env );
    /** Generate stats : **/
    int32_t generate( ENVIRONMENT* env );
};

#endif // PWX_MAKESIMPLEXTEXTURE_TEXTBOX_H_INCLUDED
