/** @file tools.cpp
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


#include "tools.h"
#include "icon.h"

int32_t initSFML( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    // Create Window:
    string title = "MakeSimplexTexture V";
    title += env->getVersion();
    title += " (c) PrydeWorX 2007-2012";
    PWX_TRY( env->screen = new sf::RenderWindow( sf::VideoMode( env->scrWidth, env->scrHeight ), title ) )
    catch ( std::bad_alloc& e ) {
        result = EXIT_FAILURE;
        cerr << "Unable to init SFML RenderWindow: \"" << e.what() << "\"" << endl;
    }

    // Set Icon
    env->screen->setIcon( mkst_icon.width, mkst_icon.height, mkst_icon.pixel_data );

    // Load font:
    if ( EXIT_SUCCESS == result ) {
        std::string fontpath = FONT_PATH;
        fontpath += FONT_SEP;
        fontpath += FONT_NAME;

        if ( fontpath.size() > 2 ) {
            try {
                env->font = new sf::Font;
                if ( env->font->loadFromFile( std::string( fontpath ) ) )
                    env->fontSize = 12;
                else {
                    cerr << "Failed to load \"" << FONT_PATH << FONT_SEP << FONT_NAME << "\"\n";
                    if ( env->font ) delete env->font;
                }
            } catch ( std::bad_alloc& e ) {
                cerr << "Error creating font : " << e.what() << endl;
            }
        } else
            cerr << "No font specified!\n";

        if ( !env->font ) {
            cerr << "No font is bad!" << endl;
            return EXIT_FAILURE;
        }
    }

    // finally set the images to our screen width and height:
    if ( EXIT_SUCCESS == result ) {
        env->image.create  ( env->scrWidth, env->scrHeight );
        env->bumpMap.create( env->scrWidth, env->scrHeight );
    }

    return ( result );
}


int32_t  parseHex( const char* src ) {
    int32_t result = 0;

    if ( src ) {
        int32_t pos    = ( ( src[0] == '0' ) && ( src[1] == 'x' ) ) ? 2 : 0;
        int32_t length = strlen( src ) - pos;

        if ( length > 8 )
            length = 8; // because 8 * 4 bit shift = 32 bit

        while ( length ) {
            --length; // dec first, or everything will be shifted left 4 bit
            if ( ( src[pos] > 0x2f ) && ( src[pos] < 0x3a ) )
                result |= ( src[pos] - 0x30 ) << ( 4 * length ); // Add number
            else if ( ( src[pos] > 0x40 ) && ( src[pos] < 0x47 ) )
                result |= ( src[pos] - 0x37 ) << ( 4 * length ); // Add upper case letter
            else if ( ( src[pos] > 0x60 ) && ( src[pos] < 0x67 ) )
                result |= ( src[pos] - 0x57 ) << ( 4 * length ); // Add lower case letter
            else
                length = 0; // break here!
            ++pos;
        }
    }

    return ( result );
}
