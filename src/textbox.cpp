/** @file textbox.cpp
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


#include <cassert>

#include "textbox.h"


int32_t CStatsText::generate( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;
    char chText[256];
    char chSeqZ[40];
    char chSeqW[40];

    memset ( chText, 0, 256 );
    memset ( chSeqZ, 0,  40 );
    memset ( chSeqW, 0,  40 );

    /// Step one: Generate the text content
    txtContent[0].setString( chText );

    if ( env->withBump )
        snprintf( chText, 255, " Texture %08d (with Bumpmap)", env->imageNum );
    else
        snprintf( chText, 255, " Texture %08d (no Bumpmap)", env->imageNum );
    txtContent[1].setString( chText );
    snprintf( chText, 255, " Seed : %d, Dimensions : %d", env->seed, env->dimensions );
    txtContent[2].setString( chText );
    snprintf( chText, 255, " Offset X : %g", env->offX );
    txtContent[3].setString( chText );
    snprintf( chText, 255, " Offset Y : %g", env->offY );
    txtContent[4].setString( chText );

    if ( env->dimensions >= 3 ) {
        snprintf( chText, 255, " Offset Z : %g (mod Z : %g)", env->offZ, env->modZ );
        showSeqPat( env, chSeqZ, chSeqW );
    } else
        memset ( chText, 0, 256 );
    txtContent[5].setString( chText );
    txtContent[6].setString( "" );

    if ( env->dimensions >= 4 )
        snprintf( chText, 255, " Offset W : %g (mod W : %g)", env->offW, env->modW );
    else
        memset ( chText, 0, 256 );
    txtContent[7].setString( chText );
    txtContent[8].setString( "" );

    snprintf( chText, 255, " Interval: %g -> %g", env->borderLo, env->borderHi );
    txtContent[9].setString( chText );


    if ( env->spxWave > 1 )
        snprintf( chText, 255, " Waves %d with reduction %g", env->spxWave, env->spxRedu );
    else
        memset ( chText, 0, 256 );
    txtContent[10].setString( chText );

    snprintf( chText, 255, " Zoom %g, Smoothing: %g", env->spxZoom, env->spxSmoo );
    txtContent[11].setString( chText );

    memset ( chText, 0, 256 );
    txtContent[12].setString( chText );

    /// Step two: set background image
    uint32_t txtWidth = 0;
    uint32_t txtTop   = env->scrHeight;
    uint32_t txtBottom= 0;
    for ( int32_t i = 0; i < txtSize; ++i ) {
        sf::FloatRect txtRect = txtContent[i].getGlobalBounds();
        txtWidth   = std::max( txtWidth, static_cast<uint32_t>( std::round( txtRect.width ) ) );
        txtTop     = std::min( txtTop,   static_cast<uint32_t>( std::round( txtRect.top ) ) );
        txtBottom  = std::max( txtBottom,static_cast<uint32_t>( std::round( txtRect.top + txtRect.height ) ) );
    }

    try {
        if ( txtBox )
            delete ( txtBox );
        txtBox = new sf::Image;
        txtBox->create( txtWidth + 10, txtBottom - txtTop + 10, bgColor );
    } catch ( std::bad_alloc& e ) {
        cerr << "ERROR  : Can't create background sprite for the stats text!" << endl;
        cerr << "Reason : \"" << e.what() << "\"" << endl;
        result = EXIT_FAILURE;
    }


    return ( result );
}


int32_t CStatsText::initialize( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    /// Step one: set colors:
    uint8_t redMin = std::max( env->colHi.r, std::max( env->colMid.r, env->colLow.r  ) );
    uint8_t greMin = std::max( env->colHi.g, std::max( env->colMid.g, env->colLow.g ) );
    uint8_t bluMin = std::max( env->colHi.b, std::max( env->colMid.b, env->colLow.b ) );
    uint8_t redMax = std::min( env->colHi.r, std::min( env->colMid.r, env->colLow.r  ) );
    uint8_t greMax = std::min( env->colHi.g, std::min( env->colMid.g, env->colLow.g ) );
    uint8_t bluMax = std::min( env->colHi.b, std::min( env->colMid.b, env->colLow.b ) );
    // Set the foreground text color:
    txtColor.r = redMin;
    txtColor.g = greMin;
    txtColor.b = bluMin;
    // Set the Background color:
    bgColor.r  =redMax;
    bgColor.g  =greMax;
    bgColor.b  =bluMax;

    /// Step two: Set size and allocate memory:
    txtSize = 13;
    try {
        txtContent = new sf::Text[txtSize];
        for ( int32_t i = 0; i < txtSize; ++i ) {
            txtContent[i].setFillColor( txtColor );
            txtContent[i].setFont( *( env->font ) );
            txtContent[i].setPosition( 10.f, 10.f + ( i * 15 ) );
            txtContent[i].setCharacterSize( env->fontSize );
            txtContent[i].setString( "" );
        }
    } catch ( std::bad_alloc& e ) {
        cerr << "ERROR  : Can't create " << txtSize << " strings for status text!" << endl;
        cerr << "Reason : \"" << e.what() << "\"" << endl;
        result = EXIT_FAILURE;
    }

    // no set text, texts are generated by generate()
    // the same applies for the background image, as we have no dimensions yet.

    return ( result );
}


int32_t CHelpText::generate( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    /// step one: Generate Text:
    int32_t i = 0;
    txtContent[i++].setString      ( "                                                                " );
    txtContent[i++].setString      ( " Key Mappings :                                                 " );
    txtContent[i++].setString      ( " -------------------------------------------------------------- " );
    txtContent[i++].setString      ( " CURSOR: modify x with left/right, y with up/down cursor keys   " );
    txtContent[i++].setString      ( "       : left/down decreases, right/up increases. (default 1.0) " );
    txtContent[i++].setString      ( "         press ctrl to increase/decrease with 10 times mod x/y. " );
    txtContent[i++].setString      ( " d/D   : increase/decrease dimensions (2-4)                     " );
    txtContent[i++].setString      ( " ESC   : Quit program                                           " );
    txtContent[i++].setString      ( " h     : Show this help                                         " );
    txtContent[i++].setString      ( " R     : Render image with current settings                     " );
    if ( env->withBump ) {
        txtContent[i++].setString  ( " s     : Save current texture and bumpmap                       " );
        txtContent[i++].setString  ( " SPACE : Toggle between texture and bumpmap                     " );
    } else {
        txtContent[i++].setString  ( " s     : Save current texture                                   " );
        txtContent[i++].setString  ( "                                                                " );
    }
    txtContent[i++].setString      ( " TAB   : Show stats of the current image                        " );
    if ( env->dimensions >= 4 ) {
        txtContent[i++].setString  ( " w/W   : increase/decrease offset w by mod w. (defaults to 1.0) " );
        txtContent[i++].setString  ( "         press ctrl to increase/decrease with 10 times mod w.   " );
        txtContent[i++].setString  ( "         press alt to toggle w coordinate sequence pattern.     " );
    } else {
        txtContent[i++].setString  ( "                                                                " );
        txtContent[i++].setString  ( "                                                                " );
        txtContent[i++].setString  ( "                                                                " );
    }
    if ( env->dimensions >= 3 ) {
        txtContent[i++].setString  ( " z/Z   : increase/decrease offset z by mod z. (defaults to 1.0) " );
        txtContent[i++].setString  ( "         press ctrl to increase/decrease with 10 times mod z.   " );
        txtContent[i++].setString  ( "         press alt to toggle z coordinate sequence pattern.     " );
    } else {
        txtContent[i++].setString  ( "                                                                " );
        txtContent[i++].setString  ( "                                                                " );
        txtContent[i++].setString  ( "                                                                " );
    }
    txtContent[i++].setString      ( "                                                                " );

    assert( i == txtSize );

    /// Step two: get text width and height:
    uint32_t txtWidth = 0;
    uint32_t txtTop   = env->scrHeight;
    uint32_t txtBottom= 0;
    for ( i = 0; i < txtSize; ++i ) {
        sf::FloatRect txtRect = txtContent[i].getGlobalBounds();
        txtWidth   = std::max( txtWidth, static_cast<uint32_t>( std::round( txtRect.width ) ) );
        txtTop     = std::min( txtTop,   static_cast<uint32_t>( std::round( txtRect.top ) ) );
        txtBottom  = std::max( txtBottom,static_cast<uint32_t>( std::round( txtRect.height - txtRect.top ) ) );
    }

    /// Step three: Generate Background image:
    try {
        if ( txtBox )
            delete txtBox;
        txtBox = new sf::Image;
        txtBox->create( txtWidth + 10, txtBottom - txtTop + 10, bgColor );
    } catch ( std::bad_alloc& e ) {
        cerr << "ERROR  : Can't create background image for the help text!" << endl;
        cerr << "Reason : \"" << e.what() << "\"" << endl;
        result = EXIT_FAILURE;
    }

    return ( result );
}


int32_t CHelpText::initialize( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    /// Step one: set colors:
    uint8_t redMin = std::max( env->colHi.r, std::max( env->colMid.r, env->colLow.r  ) );
    uint8_t greMin = std::max( env->colHi.g, std::max( env->colMid.g, env->colLow.g ) );
    uint8_t bluMin = std::max( env->colHi.b, std::max( env->colMid.b, env->colLow.b ) );
    uint8_t redMax = std::min( env->colHi.r, std::min( env->colMid.r, env->colLow.r  ) );
    uint8_t greMax = std::min( env->colHi.g, std::min( env->colMid.g, env->colLow.g ) );
    uint8_t bluMax = std::min( env->colHi.b, std::min( env->colMid.b, env->colLow.b ) );
    // Set the foreground text color:
    txtColor.r = redMax;
    txtColor.g = greMax;
    txtColor.b = bluMax;
    // Set the Background color:
    bgColor.r  =redMin;
    bgColor.g  =greMin;
    bgColor.b  =bluMin;

    /// Step two: Set size and allocate memory:
    txtSize = 20;
    try {
        txtContent = new sf::Text[txtSize];
        for ( int32_t i = 0; i < txtSize; ++i ) {
            txtContent[i].setFillColor( txtColor );
            txtContent[i].setFont( *( env->font ) );
            txtContent[i].setPosition( 10.f, 10.f + ( i * 15 ) );
            txtContent[i].setCharacterSize( env->fontSize );
            txtContent[i].setString( "" );
        }
    } catch ( std::bad_alloc& e ) {
        cerr << "ERROR  : Can't create " << txtSize << " strings for help text!" << endl;
        cerr << "Reason : \"" << e.what() << "\"" << endl;
        result = EXIT_FAILURE;
    }

    // The background image is calcualted by generate().

    return ( result );
}


/** @brief ~CTextBox
  *
  * default dtor, frees resources.
  */
CTextBox::~CTextBox() {
    // free created text surface if appropriate
    if ( txtContent )
        delete [] txtContent;
    txtContent = NULL;

    // delete text box image
    if ( txtBox )
        delete txtBox;
    txtBox = NULL;
}

/** @brief CTextBox
  *
  * default constructor, nothing special
  */
CTextBox::CTextBox():
    bgColor( 200, 160, 120 ),txtColor( 0, 0, 0 ),
    txtBox( NULL ),txtSize( 0 ),txtContent( NULL ) {
    /* Nothing to be done here... */
}
