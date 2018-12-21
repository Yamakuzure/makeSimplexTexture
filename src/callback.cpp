/** @file callback.cpp
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


#include <pwx_compiler.h>
#include <pwx_debug.h>
#include <pwx_macros.h>
#include <pwxStreamHelpers.h>
#include <string>

#include "callback.h"
#include "environment.h"
#include "consoleui.h"


// Global env pointer for our callback.
ENVIRONMENT* global_env = nullptr;

void argCallBack ( char const* arg, char const* val ) {
    if ( STREQ( "--help", arg ) ) {
        showHelp( global_env );
        global_env->doWork = false;
    } else if ( STREQ( "--version", arg ) ) {
        showVersion( global_env );
        global_env->doWork = false;
    } else if ( STREQ( "-H", arg ) || STREQ( "--colhi", arg ) ) {
        uint32_t value = std::stoul( val, nullptr, 16 );
        DEBUG_LOG( "argCallBack", "HiCol: Converted \"%s\" into %u\n", val, value );
        global_env->colHi.r = static_cast<uint8_t>( ( value & 0xff0000 ) >> 16 );
        global_env->colHi.g = static_cast<uint8_t>( ( value & 0x00ff00 ) >>  8 );
        global_env->colHi.b = static_cast<uint8_t>( value & 0x0000ff );
    } else if ( STREQ( "-L", arg ) || STREQ( "--collow", arg ) ) {
        uint32_t value = std::stoul( val, nullptr, 16 );
        DEBUG_LOG( "argCallBack", "LoCol: Converted \"%s\" into %u\n", val, value );
        global_env->colLow.r = static_cast<uint8_t>( ( value & 0xff0000 ) >> 16 );
        global_env->colLow.g = static_cast<uint8_t>( ( value & 0x00ff00 ) >>  8 );
        global_env->colLow.b = static_cast<uint8_t>( value & 0x0000ff );
        global_env->colLowEmpty = true;
    } else if ( STREQ( "-M", arg ) || STREQ( "--colmid", arg ) ) {
        uint32_t value = std::stoul( val, nullptr, 16 );
        DEBUG_LOG( "argCallBack", "MiCol: Converted \"%s\" into %u\n", val, value );
        global_env->colMid.r = static_cast<uint8_t>( ( value & 0xff0000 ) >> 16 );
        global_env->colMid.g = static_cast<uint8_t>( ( value & 0x00ff00 ) >>  8 );
        global_env->colMid.b = static_cast<uint8_t>( value & 0x0000ff );
        global_env->colMidEmpty = false;
    } else if ( STREQ( "--3D", arg ) )
        global_env->dimensions = 3;
    else if ( STREQ( "--4D", arg ) )
        global_env->dimensions = 4;
    else {
        bool need4D = false;
        if ( STREQ( arg, "zIncModX" ) )
            global_env->seqZ = eiZincX;
        else if ( STREQ( arg, "zIncModY" ) )
            global_env->seqZ = eiZincY;
        else if ( STREQ( arg, "zIncModW" ) ) {
            need4D     = true;
            global_env->seqZ = eiZincW;
        } else if ( STREQ( arg, "zIsX" ) )
            global_env->seqZ = eiZisX;
        else if ( STREQ( arg, "zIsY" ) )
            global_env->seqZ = eiZisY;
        else if ( STREQ( arg, "zIsW" ) ) {
            need4D     = true;
            global_env->seqZ = eiZisW;
        } else if ( STREQ( arg, "zIsXY" ) )
            global_env->seqZ = eiZisXY;
        else if ( STREQ( arg, "zIsXYdW" ) ) {
            need4D     = true;
            global_env->seqZ = eiZisXYdW;
        } else if ( STREQ( arg, "zIsXYmW" ) ) {
            need4D     = true;
            global_env->seqZ = eiZisXYmW;
        } else if ( STREQ( arg, "zIsXdY" ) )
            global_env->seqZ = eiZisXdY;
        else if ( STREQ( arg, "zIsYdX" ) )
            global_env->seqZ = eiZisYdX;
        else if ( STREQ( arg, "zIsXaY" ) )
            global_env->seqZ = eiZisXaY;
        else if ( STREQ( arg, "zIsXsY" ) )
            global_env->seqZ = eiZisXsY;
        else if ( STREQ( arg, "zIsYsX" ) )
            global_env->seqZ = eiZisYsX;
        else if ( STREQ( arg, "zIsXmY" ) )
            global_env->seqZ = eiZisXmY;
        else if ( STREQ( arg, "zIsYmX" ) )
            global_env->seqZ = eiZisYmX;
        // Sequence modifiers for Coordinate W
        else if ( STREQ( arg, "wIncModX" ) ) {
            need4D     = true;
            global_env->seqW = eiWincX;
        } else if ( STREQ( arg, "wIncModY" ) ) {
            need4D     = true;
            global_env->seqW = eiWincY;
        } else if ( STREQ( arg, "wIncModZ" ) ) {
            need4D     = true;
            global_env->seqW = eiWincZ;
        } else if ( STREQ( arg, "wIsX" ) ) {
            need4D     = true;
            global_env->seqW = eiWisX;
        } else if ( STREQ( arg, "wIsY" ) ) {
            need4D     = true;
            global_env->seqW = eiWisY;
        } else if ( STREQ( arg, "wIsZ" ) ) {
            need4D     = true;
            global_env->seqW = eiWisZ;
        } else if ( STREQ( arg, "wIsXY" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXY;
        } else if ( STREQ( arg, "wIsXYdZ" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXYdZ;
        } else if ( STREQ( arg, "wIsXYmZ" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXYmZ;
        } else if ( STREQ( arg, "wIsXdY" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXdY;
        } else if ( STREQ( arg, "wIsYdX" ) ) {
            need4D     = true;
            global_env->seqW = eiWisYdX;
        } else if ( STREQ( arg, "wIsXaY" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXaY;
        } else if ( STREQ( arg, "wIsXsY" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXsY;
        } else if ( STREQ( arg, "wIsYsX" ) ) {
            need4D     = true;
            global_env->seqW = eiWisYsX;
        } else if ( STREQ( arg, "wIsXmY" ) ) {
            need4D     = true;
            global_env->seqW = eiWisXmY;
        } else if ( STREQ( arg, "wIsYmX" ) ) {
            need4D     = true;
            global_env->seqW = eiWisYmX;
        }

        // Now the dimensions need to be set to the required minimum:
        if ( ( global_env->dimensions < 3 ) && !need4D )
            global_env->dimensions = 3;
        else if ( ( global_env->dimensions < 4 ) && need4D )
            global_env->dimensions = 4;
    }
}

void setGlobalEnv( ENVIRONMENT* env ) {
    global_env = env;
}
