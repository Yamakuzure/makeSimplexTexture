#ifndef PWX_MAKESIMPLEXTEXTURE_ENVIRONMENT_H
#define PWX_MAKESIMPLEXTEXTURE_ENVIRONMENT_H

/** @file environment.h
 *
 * This file is part of makeSimplexTexture
 *
 * (c) 2007 - 2018 PrydeWorX
 * @author Sven Eden, PrydeWorX - Bardowick, Germany
 *         sven.eden@prydeworx.com
 *         https://github.com/Yamakuzure/makeSimplexTexture; https://prydeworx.com/makeSimplexTexture
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


#include <pwxCLockable.h>
#include <pwx_macros.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <cmath>
#include <string>
using std::string;
#include <cstdio> // for sprintf
#if defined (_MSC_VER)
#  include <io.h>   // for _access (used by pwx_file_exists())
#else
#  include <unistd.h>
#endif

class CHelpText;
class CStatsText;

#include "textbox.h" // Adds On Screen help and stats
#include "wzseq.h"

/** @struct ENVIRONMENT
  * @brief struct to keep general values together that are used in the programs functions
**/
struct ENVIRONMENT: public pwx::CLockable {
    double            borderHi    = 1.0;    //!< Upper border, defaults to the simplex maximum of  1.0
    double            borderLo    = -1.0;   //!< Lower border, defaults to the simplex minimum of -1.0
    double            borderMid   = 0.0;    //!< Middle border, set to the median of the lower and upper border
    sf::Image         bumpMap;              //!< Optional bumpmapping for the generated texture
    sf::Color         colHi       = { 255, 255, 0 };//!< Must have color, if alone uses the full range, otherwise the positive Simplex value
    sf::Color         colMid      = { 0, 0, 0 };//!< optional middle color, takes over the zero value
    sf::Color         colLow      = { 0, 0, 0 };//!< optional low color, takes over negative colors
    bool              colLowEmpty = true;   //!< set to false if the user sets a color via command line
    bool              colMidEmpty = true;   //!< set to false if the user sets a color via command line
    int32_t           dimensions  = 2;      //!< can be 2, 3 or 4
    bool              doWork      = true;   //!< is set to false if no work is to be done
    sf::Font*         font        = nullptr;//!< The font to be used for the display
    float             fontSize    = 12.f;   //!< Base size of the font, used to determine the text box sizes
    bool              helpBoxShown= false;  //!< set to true if the help box is shown
    CHelpText*        helpText    = nullptr;//!< Help text for OS display
    sf::Image         image;                //!< The image to be rendered
    int32_t           imageNum    = 0;      //!< used to count the available images, to not overwrite one
    char              imgMsg[256] = { 0 };  //!< used to display the stats of the current image
    bool              mainShown   = true;   //!< True if the texture, false if the bumpmap is shown on screen
    double            modZ        = 1.0;    //!< modification value for offZ, used for z/Z key and iterations (default 1.0)
    double            modW        = 1.0;    //!< modification value for offW, used for w/W key and iterations (default 1.0)
    char              msg[256]    = { 0 };  //!< message to display in the window title
    int32_t           msgShown    = 0;      //!< showMsg sets to -1, workLoop then triggers timer and sets to 1, showState sets to 0
    int32_t           numThreads  = 8;      //!< Number of threads to spawn for the render calculations. Default is 8
    double            offX        = 0.0;    //!< x-offset
    double            offY        = 0.0;    //!< y-offset
    double            offZ        = 0.0;    //!< z-offset enabled by dimension >= 3
    double            offW        = 0.0;    //!< w-offset enabled by dimension == 4
    bool              removeBox   = false;  //!< Set to true if a txtBox is shown than shall be removed
    sf::RenderWindow* screen      = nullptr;//!< the screen to be created
    int32_t           scrHeight   = 600;    //!< height of the screen
    int32_t           scrSize     = 480000; //!< simply a shortcut to height * width
    int32_t           scrWidth    = 800;    //!< screen width
    eSequenceW        seqW        = eiWNone;//!< determines the sequence of the w coordinate
    eSequenceZ        seqZ        = eiZNone;//!< determines the sequence of the z coordinate
    double            spxRedu     = 1.0;    //!< Simplex Reduction Value, defaults to 1.0
    double            spxSmoo     = 1.0;    //!< Simplex Smooth Value, defaults to 1.0
    int32_t           spxWave     = 1;      //!< Simplex Waves Value, defaults to 1
    double            spxZoom     = 9.0;    //!< Simplex Zoom, defaults to 4.0
    bool              statBoxShown= false;  //!< set to true if the stats box is shown
    CStatsText*       statsText   = nullptr;//!< Text to display by showOSStat()
    int32_t           seed        = 0;      //!< If set by command line argument, sets a new seed for RNG
    string            targetExt   = "png";  //!< target extension, determines image type
    sf::Texture       texture;     //!< Graphics memory to render.
    sf::Thread**      thread      = nullptr;//!< The threads themselves.
    int32_t*          threadPrg   = nullptr;//!< Threads write their progress in this
    bool*             threadRun   = nullptr;//!< Threads set it to true when they start and to false when they end
    bool              verbose     = true;   //!< if set to false (-q/--quiet) when batch mode is enabled, nothing is printed out
    bool              withBump    = true;   //!< Save a textureXXXX.bump.tif alongside the original texture. Defaults to true!
    bool              withGUI     = true;   //!< When set to false by -B/--batch option, no SDL is used

    /* --- non-struct methods --- */
    void         clearThreads();
    const char* getVersion() const;

    /** @brief setCoordsByX
      *
      * Set dX to dW according to sequences concerning x
      *
      * The relevant sequences are:
      *  eiZincX,    //!< Z = modZ + (X * modZ)
      *  eiZincW,    //!< Z = modZ + (W * modZ)
      *  eiZisX,     //!< Z equals X
      *  eiZisW,     //!< Z equals W
      *  eiZisXY,    //!< Z equals X * Y
      *  eiZisXYdW,  //!< Z equals (X * Y) / W
      *  eiZisXYmW,  //!< Z equals (X * Y) % W
      *  eiZisXdY,   //!< Z equals X / Y
      *  eiZisYdX,   //!< Z equals Y / X
      *  eiZisXaY,   //!< Z equals X + Y
      *  eiZisXsY,   //!< Z equals X - Y
      *  eiZisYsX,   //!< Z equals Y - X
      *  eiZisXmY,   //!< Z equals X % Y
      *  eiZisYmX,   //!< Z equals Y % Y
      * and the same with seqW
    **/
    void setCoordsByX( int32_t x, int32_t y, double& dX, double& dY, double& dZ, double& dW ) {
        dX = offX + static_cast<double>( x );

        if ( ( seqZ != eiZNone ) || ( seqW != eiWNone ) ) {
            bool zIsChanged = false;
            bool wIsChanged = false;
            int32_t w, xy, z;

            // 1) Process seqZ
            if ( seqZ != eiZNone ) {
                zIsChanged = true;
                switch ( seqZ ) {
                    case eiZincX:
                        dZ = modZ + ( modZ * dX );
                        break;
                    case eiZisX:
                        dZ = dX;
                        break;
                    case eiZisXY:
                        dZ = offZ + ( dX * dY );
                        break;
                    case eiZisXYdW:
                        dZ = offZ + ( dW ? ( dX * dY ) / dW : 0 );
                        break;
                    case eiZisXYmW:
                        w  = static_cast<int32_t>( std::round( dW ) );
                        xy = static_cast<int32_t>( std::round( dX * dY ) );
                        dZ = offZ + ( w ? static_cast<double>( xy % w ) : 0 );
                        break;
                    case eiZisXdY:
                        dZ = offZ + ( y ? static_cast<double>( x ) / static_cast<double>( y ) : 0 );
                        break;
                    case eiZisYdX:
                        dZ = offZ + ( x ? static_cast<double>( y ) / static_cast<double>( x ) : 0 );
                        break;
                    case eiZisXaY:
                        dZ = offZ + dX + dY;
                        break;
                    case eiZisXsY:
                        dZ = offZ + ( dX - dY );
                        break;
                    case eiZisYsX:
                        dZ = offZ + ( dY - dX );
                        break;
                    case eiZisXmY:
                        dZ = offZ + ( y > 0 ? static_cast<double>( x % y ) : 0 );
                        break;
                    case eiZisYmX:
                        dZ = offZ + ( x > 0 ? static_cast<double>( y % x ) : 0 );
                        break;
                    // The next two are processed later if, and only if, W is modified
                    case eiZincW:
                    case eiZisW:
                    // the next two are already processed by setCoordsByY()
                    case eiZincY:
                    case eiZisY:
                    default:
                        zIsChanged = false;
                        break;
                } // end switch (seqZ)
            } // End of seqZ processing

            // 2) Process seqW
            if ( seqW != eiWNone ) {
                wIsChanged = true;
                switch ( seqW ) {
                    case eiWincX:
                        dW = modW + ( dX * modW );
                        break;
                    case eiWisX:
                        dW = dX;
                        break;
                    case eiWisXY:
                        dW = offW + ( dX * dY );
                        break;
                    case eiWisXYdZ:
                        dW = offW + ( dZ ? ( dX * dY ) / dZ : 0 );
                        break;
                    case eiWisXYmZ:
                        z  = static_cast<int32_t>( std::round( dZ ) );
                        xy = static_cast<int32_t>( std::round( dX * dY ) );
                        dW = offW + ( z ? static_cast<double>( xy % z ) : 0 );
                        break;
                    case eiWisXdY:
                        dW = offW + ( y ? static_cast<double>( x ) / static_cast<double>( y ) : 0 );
                        break;
                    case eiWisYdX:
                        dW = offW + ( x ? static_cast<double>( y ) / static_cast<double>( x ) : 0 );
                        break;
                    case eiWisXaY:
                        dW = offW + dX + dY;
                        break;
                    case eiWisXsY:
                        dW = offW + ( dX - dY );
                        break;
                    case eiWisYsX:
                        dW = offW + ( dY - dX );
                        break;
                    case eiWisXmY:
                        dW = offW + ( y > 0 ? static_cast<double>( x % y ) : 0 );
                        break;
                    case eiWisYmX:
                        dW = offW + ( x > 0 ? static_cast<double>( y % x ) : 0 );
                        break;
                    case eiWincZ:
                        if ( zIsChanged )
                            dW = modW + ( dZ * modW );
                        else
                            wIsChanged = false;
                        break;
                    case eiWisZ:
                        if ( zIsChanged )
                            dW = dZ;
                        else
                            wIsChanged = false;
                        break;
                    // the next two are already processed by setCoordsByY()
                    case eiWincY:
                    case eiWisY:
                    default:
                        wIsChanged = false;
                        break;
                } // end switch (seqW)
            } // End of seqW processing

            // 3) Mod Z if W is changed
            if ( wIsChanged && ( seqZ == eiZincW ) )
                dZ = modZ + ( dW * modZ );
            else if ( wIsChanged && ( seqZ == eiZisW ) )
                dZ = dW;

            // Processing of W if Z changed is already done in 2)

        } // End processing either seqZ, seqW or both
    }

    /** @brief setCoordsByY
      *
      * Set dY, dZ, dW according to sequences concerning y
      * This is the very limited version for the outer loop, excluding x
      *
      * The relevant sequences are:
      *  eiZincY,    //!< Z = modZ + (Y * modZ)
      *  eiZincW,    //!< Z = modZ + (W * modZ)
      *  eiZisY,     //!< Z equals Y
      *  eiZisW,     //!< Z equals W
      * and the same with seqW
    **/
    void setCoordsByY( int32_t y, double& dY, double& dZ, double& dW ) {
        dY = offY + static_cast<double>( y );
        if ( ( seqZ != eiZNone ) || ( seqW != eiWNone ) ) {
            bool zIsChanged = false;
            bool wIsChanged = false;

            // 1) Use seqZ
            if ( seqZ == eiZincY ) {
                dZ = modZ + ( dY * modZ );
                zIsChanged = true;
            } else if ( seqZ == eiZisY ) {
                dZ = dY;
                zIsChanged = true;
            }

            // 2) Use seqW
            if ( seqW == eiWincY ) {
                dW = modW + ( dY * modW );
                wIsChanged = true;
            } else if ( seqW == eiWisY ) {
                dW = dY;
                wIsChanged = true;
            }

            // 3) Mod Z if W is changed
            if ( wIsChanged && ( seqZ == eiZincW ) )
                dZ = modZ * ( dW * modZ );
            else if ( wIsChanged && ( seqZ == eiZisW ) )
                dZ = dW;

            // 4) Mod W if Z is changed
            if ( zIsChanged && ( seqW == eiWincZ ) )
                dW = modW + ( dZ * modW );
            else if ( zIsChanged && ( seqW == eiWisZ ) )
                dW = dZ;
        }
        // That's it.
    }

    /** @brief setImageNum
      *
      * Just a little method that sets imageNum to the lowest possible value with
      * the current targetExt
    **/
    void setImageNum() {
        char fname[32] = "";
        imageNum = 0;
        do
            snprintf( fname, 31, "texture%08d.%s", ++imageNum, targetExt.c_str() );
        while ( pwx_file_exists( fname ) );
    }

  private:
    string version = VERSION;

  public:
    /** @brief Default constructor **/
    explicit ENVIRONMENT( int32_t aSeed = 0 )
        : seed( aSeed ) {
        memset( msg, 0, 256 );
        memset( imgMsg, 0, 256 );

        // Now find first available imagenum:
        setImageNum();
    }

    // The dtor has to be outline, or we can't get rid of the forward declared text boxes.
    ~ENVIRONMENT();

  private:
    /* --- absolutely no copying allowed --- */
    ENVIRONMENT( ENVIRONMENT& );
    ENVIRONMENT& operator=( ENVIRONMENT& );
};

/// @brief tiny little helpers to let threads know their number
struct threadEnv {
    ENVIRONMENT* env;
    int32_t      threadNum;   //!< This is used to tell threads which number they are for displaying the progress

    explicit threadEnv( ENVIRONMENT* aEnv, int32_t aNum ):env( aEnv ),threadNum( aNum ) {}
    ~threadEnv() { env = NULL; }

  private:
    /* --- do not copy --- */
    threadEnv( threadEnv& );
    threadEnv& operator=( threadEnv& );
};

#endif // PWX_MAKESIMPLEXTEXTURE_ENVIRONMENT_H
