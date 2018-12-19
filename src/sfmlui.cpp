/** @file sfmlui.cpp
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


#include "sfmlui.h"
#include <cstdarg>


int32_t saveTexture( ENVIRONMENT* env ) {
    int32_t result    = EXIT_SUCCESS;
    char    fname[32] = "";

    snprintf( fname, 31, "texture%08d.%s", env->imageNum, env->targetExt.c_str() );
    showMsg( env, "saving texture %d ...", env->imageNum );

    env->image.saveToFile( std::string( fname ) );
    if ( env->withBump ) {
        snprintf( fname, 31, "texture%08d.bump.%s", env->imageNum, env->targetExt.c_str() );
        env->bumpMap.saveToFile( std::string( fname ) );
    }

    showMsg( env, "texture %d saved!", env->imageNum++ );
    if ( !env->withGUI )
        cout << endl;

    return ( result );
}


void showMsg( ENVIRONMENT* env, const char* fmt, ... ) {
    if ( !env->withGUI && env->verbose ) {
        memset( env->msg, '\b', strlen( env->msg ) );
        cout << env->msg;
    }

    va_list vl;
    va_start( vl, fmt );
    vsnprintf( env->msg, 255, fmt, vl );
    va_end( vl );

    if ( env->withGUI ) {
        env->texture.loadFromImage( env->image );
        env->screen->clear();
        env->screen->draw( sf::Sprite( env->texture ) );

        // Set the text first, we need its size
        sf::Text sMsg( env->msg, *( env->font ), env->fontSize );
        sMsg.setFillColor( sf::Color( ::std::min( env->colHi.r, ::std::min( env->colMid.r, env->colLow.r ) ),
                                      ::std::min( env->colHi.g, ::std::min( env->colMid.g, env->colLow.g ) ),
                                      ::std::min( env->colHi.b, ::std::min( env->colMid.b, env->colLow.b ) ) ) );
        sf::FloatRect txtRect( sMsg.getGlobalBounds() );

        // Draw the text background box:
        uint32_t bWidth = static_cast<uint32_t>( std::round( txtRect.width + 4 ) );
        uint32_t bHeight= static_cast<uint32_t>( std::round( txtRect.height + 4 ) );
        sf::IntRect boxPos(  static_cast<uint32_t>( env->scrWidth  / 2 - bWidth / 2 - 2 ),
                             static_cast<uint32_t>( env->scrHeight - 2 - bHeight ),
                             bWidth, bHeight );
        sf::Vector2f txtPos( static_cast<float>( env->scrWidth  / 2 - bWidth / 2    ),
                             static_cast<float>( env->scrHeight     - bHeight ) );
        sf::Image  boxBg;
        boxBg.create( bWidth, bHeight, sf::Color(
                          ::std::max( env->colHi.r, ::std::max( env->colMid.r, env->colLow.r ) ),
                          ::std::max( env->colHi.g, ::std::max( env->colMid.g, env->colLow.g ) ),
                          ::std::max( env->colHi.b, ::std::max( env->colMid.b, env->colLow.b ) ) ) );
        sf::Texture texBg;
        texBg.loadFromImage( boxBg );
        sf::Sprite box( texBg, boxPos );
        env->screen->draw( box );

        // Set the text position:
        sMsg.setPosition( txtPos );

        // Draw the text and display it:
        env->screen->draw( sMsg );
        env->screen->display();

        env->msgShown = -1;
    } else if ( env->verbose ) {
        cout << env->msg;
        cout.flush();
    }
}


int32_t showOSHelp( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    // We create the text surface on demand and keep it then
    if ( !env->helpText ) {
        try {
            // Create the helpText:
            env->helpText = new CHelpText();
            result = env->helpText->initialize( env );
        } catch ( std::bad_alloc& e ) {
            cerr << "Error creating help text : \"" << e.what() << "\"" << endl;
            result = EXIT_FAILURE;
        }
    }

    // We have a help text?
    if ( ( EXIT_SUCCESS == result ) && env->helpText ) {
        // We have it, tell environment to display it:
        env->helpText->generate( env );
        env->helpBoxShown = true;
    }

    return ( result );
}


int32_t showOSStat( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    // We create the text surface on demand and keep it then
    if ( !env->statsText ) {
        try {
            // Create the statsText:
            env->statsText = new CStatsText();
            result = env->statsText->initialize( env );
        } catch ( std::bad_alloc& e ) {
            cerr << "Error creating stats text : \"" << e.what() << "\"" << endl;
            result = EXIT_FAILURE;
        }
    }

    // We have the On Screen Display?
    if ( ( EXIT_SUCCESS == result ) && env->statsText ) {
        env->statsText->generate( env );
        // We have it, tell environment to display it:
        env->statBoxShown = true;
    }

    return ( result );
}


void showSeqPat( ENVIRONMENT* env, char* seqZtgt, char* seqWtgt ) {
    char chMsgZ[36] = "";
    char chMsgW[36] = "";

    if ( env->dimensions >= 3 ) {
        // create string for Sequence Z
        switch ( env->seqZ ) {
            case eiZNone:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"static %g\"", static_cast<int32_t>( env->seqZ ), env->offZ );
                break;
            case eiZincX:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"inc %g by X\"", static_cast<int32_t>( env->seqZ ), env->modZ );
                break;
            case eiZincY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"inc %g by Y\"", static_cast<int32_t>( env->seqZ ), env->modZ );
                break;
            case eiZincW:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"inc %g by W\"", static_cast<int32_t>( env->seqZ ), env->modZ );
                break;
            case eiZisX:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals X\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals Y\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals X * Y\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXYdW:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals (X * Y) / W\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXYmW:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals (X * Y) %% W\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXdY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals X / Y\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisYdX:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals Y / X\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXaY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals X + Y\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXsY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals X - Y\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisYsX:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals Y - X\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisXmY:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals X %% Y\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisYmX:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals Y %% X\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZisW:
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"equals W\"", static_cast<int32_t>( env->seqZ ) );
                break;
            case eiZlast:
            default:
                cerr << "ERROR: seqZ reached unusable helper state!" << endl;
                env->seqZ = eiZNone;
                snprintf( chMsgZ, 35, "Seq Z: % 2d \"static %g\"", static_cast<int32_t>( env->seqZ ), env->offZ );
        } // End of Sequence Z string creation
    } // End check for third dimension

    if ( env->dimensions >= 4 ) {
        // create string for Sequence W
        switch ( env->seqW ) {
            case eiWNone:
                snprintf( chMsgW, 35, "Seq W: % 2d \"static %g\"", static_cast<int32_t>( env->seqW ), env->offW );
                break;
            case eiWincX:
                snprintf( chMsgW, 35, "Seq W: % 2d \"inc %g by X\"", static_cast<int32_t>( env->seqW ), env->modW );
                break;
            case eiWincY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"inc %g by Y\"", static_cast<int32_t>( env->seqW ), env->modW );
                break;
            case eiWincZ:
                snprintf( chMsgW, 35, "Seq W: % 2d \"inc %g by Z\"", static_cast<int32_t>( env->seqW ), env->modW );
                break;
            case eiWisX:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals X\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals Y\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals X * Y\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXYdZ:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals (X * Y) / Z\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXYmZ:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals (X * Y) %% Z\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXdY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals X / Y\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisYdX:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals Y / X\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXaY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals X + Y\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXsY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals X - Y\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisYsX:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals Y - X\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisXmY:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals X %% Y\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisYmX:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals Y %% X\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWisZ:
                snprintf( chMsgW, 35, "Seq W: % 2d \"equals Z\"", static_cast<int32_t>( env->seqW ) );
                break;
            case eiWlast:
            default:
                cerr << "ERROR: seqW reached unusable helper state!" << endl;
                env->seqW = eiWNone;
                snprintf( chMsgW, 32, "Seq W: %d \"static %g\"", static_cast<int32_t>( env->seqW ), env->offW );
        } // End of Sequence W string creation

    } // end check for fourth dimension

    // Now work with the result(s):
    if ( seqWtgt && seqZtgt ) {
        if ( env->dimensions >= 3 )
            snprintf( seqZtgt, 255, "  -> %s", chMsgZ );
        if ( env->dimensions >= 4 )
            snprintf( seqWtgt, 255, "  -> %s", chMsgW );
    } else if ( env->dimensions >= 4 )
        // Show both, we are 4D
        showMsg( env, "%s - %s", chMsgZ, chMsgW );
    else
        // Show Z only, we are 3D
        showMsg( env, "%s", chMsgZ );
}

void showState( ENVIRONMENT* env ) {
    snprintf( env->imgMsg, 255, "Texture %08d : %dx%d - Seed %d, Zoom %g, Smooth %g",
              env->imageNum, env->scrWidth, env->scrHeight, env->seed, env->spxZoom, env->spxSmoo );
    if ( env->spxWave > 1 ) {
        snprintf( env->msg, 255, "(%d waves - reduct %g)", env->spxWave, env->spxRedu );
        showMsg( env, "%s %s", env->imgMsg, env->msg );
    } else
        showMsg( env, "%s", env->imgMsg );
    env->msgShown = -1;
}
