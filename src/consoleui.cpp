/** @file consoleui.cpp
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


#include <pwx_worker_PAH.h>
#include <pwxStreamHelpers.h>


#include "consoleui.h"
#include "callback.h"

using pwx::PAH;
using pwx::eArgTargetType::ATT_SET;
using pwx::eArgTargetType::ATT_TRUE;


int32_t processArguments( ENVIRONMENT* env, int argc, char* argv[] ) {
    int32_t result = EXIT_SUCCESS;
    assert( env );
    setGlobalEnv( env );

    // -- normal arguments ---
    PAH.addArg( "", "--height",       pwx::ATT_SET, &env->scrHeight, "Set window height (minimum 480)", "height" );
    PAH.addArg( "", "--help",         argCallBack, "Show this help and exit", nullptr );
    PAH.addArg( "", "--version",      argCallBack, "Show the programs version and exit", nullptr );
    PAH.addArg( "", "--width",        pwx::ATT_SET, &env->scrWidth, "Set window width (minimum 640)", "width" );
    PAH.addArg( "-e", "--ext",        pwx::ATT_SET, &env->targetExt, "Set the file extension to the desired image format. The default is \"png\".", "ext" );
    PAH.addArg( "-l", "--bordlo",     pwx::ATT_SET, &env->borderLo, "Set low border (-1.0 to 1.0)", "value" );
    PAH.addArg( "-h", "--bordhi",     pwx::ATT_SET, &env->borderHi, "Set high border (-1.0 to 1.0)", "value" );
    PAH.addArg( "-n", "--no-bumpmap", pwx::ATT_TRUE, &env->withBump, "Omit the calculation of a bumpmap", nullptr );
    PAH.addArg( "-q", "--quiet",      pwx::ATT_TRUE, &env->verbose, "No output to the console", nullptr );
    PAH.addArg( "-s", "--seed",       pwx::ATT_SET, &env->seed, "Set seed", "value" );
    PAH.addArg( "-t", "--threads",    pwx::ATT_SET, &env->numThreads, "Set number of threads (minimum 4, default 8)", "num" );
    PAH.addArg( "-B", "--batch",      pwx::ATT_TRUE, &env->withGUI, "Enable batch mode (no GUI)", nullptr );
    PAH.addArg( "-H", "--colhi",      argCallBack, "Set the high color", "value" );
    PAH.addArg( "-L", "--collow",     argCallBack, "Set the low color", "value" );
    PAH.addArg( "-M", "--colmid",     argCallBack, "Set the middle color", "value" );
    PAH.addArg( "-R", "--reduct",     pwx::ATT_SET, &env->spxRedu, "Set the reduction on each wave (minimum 1.0)", "value" );
    PAH.addArg( "-S", "--smooth",     pwx::ATT_SET, &env->spxSmoo, "Set the smoothing on each wave (minimum 1.0)", "value" );
    PAH.addArg( "-W", "--waves",      pwx::ATT_SET, &env->spxWave, "Set number of waves (minimum 1)", "value" );
    PAH.addArg( "-Z", "--zoom",      pwx::ATT_SET, &env->spxZoom, "Set the zoom factor (minimum 0.001)", "value" );

    // --- normal options with special passages in the help text ---
    PAH.addArg( "", "--3D", argCallBack, "", nullptr );
    PAH.addArg( "", "--4D", argCallBack, "", nullptr );
    PAH.addArg( "", "--modz", pwx::ATT_SET, &env->modZ, "", nullptr );
    PAH.addArg( "", "--modw", pwx::ATT_SET, &env->modW, "", nullptr );
    PAH.addArg( "-w", "", pwx::ATT_SET, &env->offW, "", nullptr );
    PAH.addArg( "-x", "", pwx::ATT_SET, &env->offX, "", nullptr );
    PAH.addArg( "-y", "", pwx::ATT_SET, &env->offY, "", nullptr );
    PAH.addArg( "-z", "", pwx::ATT_SET, &env->offZ, "", nullptr );

    // --- sequential arguments ---
    // These have a special passage in the help text, so they do not need descriptions
    PAH.addArg( "", "zIncModX", argCallBack, "", nullptr );
    PAH.addArg( "", "zIncModY", argCallBack, "", nullptr );
    PAH.addArg( "", "zIncModW", argCallBack, "", nullptr );
    PAH.addArg( "", "zIsX",     argCallBack, "", nullptr );
    PAH.addArg( "", "zIsY",     argCallBack, "", nullptr );
    PAH.addArg( "", "zIsW",     argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXY",    argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXYdW",  argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXYmW",  argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXdY",   argCallBack, "", nullptr );
    PAH.addArg( "", "zIsYdX",   argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXaY",   argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXsY",   argCallBack, "", nullptr );
    PAH.addArg( "", "zIsYsX",   argCallBack, "", nullptr );
    PAH.addArg( "", "zIsXmY",   argCallBack, "", nullptr );
    PAH.addArg( "", "zIsYmX",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIncModX", argCallBack, "", nullptr );
    PAH.addArg( "", "wIncModY", argCallBack, "", nullptr );
    PAH.addArg( "", "wIncModZ", argCallBack, "", nullptr );
    PAH.addArg( "", "wIsX",     argCallBack, "", nullptr );
    PAH.addArg( "", "wIsY",     argCallBack, "", nullptr );
    PAH.addArg( "", "wIsZ",     argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXY",    argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXYdZ",  argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXYmZ",  argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXdY",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIsYdX",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXaY",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXsY",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIsYsX",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIsXmY",   argCallBack, "", nullptr );
    PAH.addArg( "", "wIsYmX",   argCallBack, "", nullptr );

    PAH.parseArgs( argc, argv );

    if ( 0 != PAH.getErrorCount() ) {
        result = EXIT_FAILURE;
        cout << "The following errors occurred:" << endl;
        for ( int32_t i = 1; i <= PAH.getErrorCount(); ++i )
            cout << pwx::CAdjRight( 2,0 ) << i << ".: " << PAH.getErrorStr( i ) << endl;
        cout << endl;
        showHelp( env );
    } else {
        // Low and high border need to be sanitized, and the medium shortcut added:
        if ( env->borderLo > env->borderHi ) {
            double dTmp = env->borderHi;
            env->borderHi = env->borderLo;
            env->borderLo = dTmp;
        }
        env->borderMid = 0.5 * ( env->borderHi + env->borderLo );

        // If we do not have medium _and_ low color, high color needs to expand to the middle:
        if ( env->colMidEmpty && env->colLowEmpty ) {
            env->colMid.r = static_cast<uint8_t>( std::round( 0.5 * env->colHi.r ) );
            env->colMid.g = static_cast<uint8_t>( std::round( 0.5 * env->colHi.g ) );
            env->colMid.b = static_cast<uint8_t>( std::round( 0.5 * env->colHi.b ) );
            env->colMidEmpty = false;
        }

        // Set scrSize
        env->scrSize = env->scrWidth * env->scrHeight;

        // Apply numThreads for the threads and threadPrg numbers:
        try {
            env->thread    = new sf::Thread*[env->numThreads];
            env->threadPrg = new int32_t[env->numThreads];
            env->threadRun = new bool[env->numThreads];
            for ( int32_t i = 0; i < env->numThreads; ++i ) {
                env->thread[i]    = NULL;
                env->threadPrg[i] = 0;
                env->threadRun[i] = false;
            }
        } catch ( std::bad_alloc& e ) {
            result = EXIT_FAILURE;
            cout << "ERROR: unable to allocate " << env->numThreads << " integers for thread progress!";
            cout << " [" << e.what() << "]" << endl;
        }
    } // End of parsing was successful

    return ( result );
}

void showHelp( ENVIRONMENT* env ) {
    int32_t dpw = 74; // Description print width

    cout << "getMakeSimplexTexture - ";
    showVersion( env );
    cout << "------------------------";
    showVerDash( env );
    cout << endl;
    cout << "  Usage:" << endl;
    cout << "mkst [options]" << endl << endl;
    cout << "The default behavior, when no options are given, is to open" << endl;
    cout << "a 800x600 window, generate one texture, and wait for a" << endl;
    cout << "keypress." << endl;
    cout << endl << "  Options:" << endl;
    cout << "  -x|y|z|w <value>         Set offset of the specified dimension." << endl;
    cout << "                           All four dimensions default to 0.0 and use a" << endl;
    cout << "                           stepping of one per pixel, modified by zoom" << endl;
    cout << "                           which defaults to 9.0. (This means every" << endl;
    cout << "                           pixel raises x and/or y by 1.0 / 9.0) To use" << endl;
    cout << "                           z, you have to set 3D or 4D, to use w 4D is" << endl;
    cout << "                           needed. See \"sequence patterns\" below about" << endl;
    cout << "                           how to control z and w." << endl;
    cout << "      --3D|4D              Enable third/fourth dimension" << endl;
    cout << PAH.getHelpStr( "-B", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-h", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-l", dpw, 2 ) << endl;
    cout << "                           The borders are the limit under/over which" << endl;
    cout << "                           value the low and high color are set. The" << endl;
    cout << "                           middle color is always set in the middle" << endl;
    cout << "                           between those borders. The default sequence" << endl;
    cout << "                           for low->mid->high is -1.0->0.0->+1.0" << endl;
    cout << PAH.getHelpStr( "-H", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-M", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-L", dpw, 2 ) << endl;
    cout << "                           Colors are needed as 0xRRGGBB and default to" << endl;
    cout << "                           0xffff00 for colHi, 0x7f7f00 for colMid and" << endl;
    cout << "                           0x000000 for colLow" << endl;
    cout << PAH.getHelpStr( "-e", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "--height", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "--help", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-n", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-q", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-R", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-s", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-S", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-t", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "--version", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "--width", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-W", dpw, 2 ) << endl;
    cout << PAH.getHelpStr( "-Z", dpw, 2 ) << endl;
    cout << endl << "The next set of arguments can be used to change the modifiers for" << endl;
    cout << "the coordinates." << endl;
    cout << "Changing the modifier for w and/or z may lead to very strange results," << endl;
    cout << "because they are used with the *IncMod* sequence patterns, too." << endl;
    cout << "      --mod<zw> <value>  Change the modification value for z or w" << endl;
    cout << endl << "  Sequence Patterns:" << endl;
    cout << "The following options may be added to command line options or set" << endl;
    cout << "via alt-z or alt-w at runtime. w and z are static values by default." << endl;
    cout << " (to keep the output short, <xy> means either x or y.)" << endl;
    cout << "  <wz>IncMod<XYZW>         w/z is increased whenever x/y/z/w is modified" << endl;
    cout << "  <wz>Is<XYZW>             Set w/z to be equal to x/y/z/w" << endl;
    cout << "  <wz>IsXY                 Set w/z to be equal to x * y" << endl;
    cout << "  <wz>IsXYd<ZW>            Set w/z to be equal to (x * y) / <ZW>" << endl;
    cout << "  <wz>IsXYm<ZW>            Set w/z to be equal to (x * y) % <ZW>" << endl;
    cout << "  <wz>Is<XY>d<YX>          Set w/z to be either x / y or y / x" << endl;
    cout << "  <wz>IsXaY                Set w/z to be equal to x + y" << endl;
    cout << "  <wz>Is<XY>s<YX>          Set w/z to be either x - y or y - x" << endl;
    cout << "  <wz>Is<XY>m<YX>          Set w/z to be either x % y or y % x" << endl;
    cout << endl << "  GUI key mapping:" << endl;
    cout << "CURSOR: Modify x with left/right, y with up/down cursor keys" << endl;
    cout << "d/D   : Increase/decrease dimensions (2-4)" << endl;
    cout << "ESC   : Quit program" << endl;
    cout << "h     : Show on-screen help" << endl;
    cout << "R     : Render image with current settings" << endl;
    cout << "s     : Save current texture (and bumpmap unless -n is specified)" << endl;
    cout << "SPACE : Switch between texture and bumpmap. This does only work if the" << endl;
    cout << "          -n/no-bumpmap option isn't used." << endl;
    cout << "TAB   : Show stats of the current image" << endl;
    cout << "w/W   : Increase/decrease offset w by mod w. (defaults to 1.0)" << endl;
    cout << "        Press ctrl to increase/decrease with 10 times mod w." << endl;
    cout << "        Press alt to toggle w coordinate sequence setting." << endl;
    cout << "z/Z   : Increase/decrease offset z by mod z. (defaults to 1.0)" << endl;
    cout << "        Press alt to toggle z coordinate sequence setting." << endl;
}

void showVersion( ENVIRONMENT* env ) {
    cout << "Version " << env->getVersion() << endl;
}

void showVerDash( ENVIRONMENT* env ) {
    int32_t length = 8 + strlen( env->getVersion() );
    for ( int32_t i = 0; i < length; ++i )
        cout << "-";
}
