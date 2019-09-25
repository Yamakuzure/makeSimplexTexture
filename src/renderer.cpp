/** @file renderer.cpp
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


#include <chrono>
#include <PMath>
#include <RNG>
#include <thread>

#include "renderer.h"

using pwx::RNG;

int32_t loadNewImage( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    try {
        int32_t prgCurr = 0;

        // Set seed to be sure to use the right instance
        RNG.setSeed( env->seed );

        // Start with first message
        showMsg( env, "Rendering: 0%%" );

        // Fire up our threads
        for ( int32_t tNum = 0; tNum < env->numThreads; ++tNum ) {
            threadEnv* tEnv   = new threadEnv( env, tNum );
            env->thread[tNum] = new sf::Thread( thrdDraw, tEnv );
            env->thread[tNum]->launch();
        }

        // Now wait for all threads to finish showing progress
        int32_t   running = env->numThreads;
        int32_t   prgCur  = 0;
        int32_t   slept   = 0;
        sf::Clock clock;

        while ( running ) {
            // first count up what has to be done:
            prgCur = 0;
            slept  = static_cast<int32_t>( clock.getElapsedTime().asMilliseconds() );
            running= 0;
            clock.restart();

            for ( int32_t tNum = 0; tNum < env->numThreads; ++tNum ) {
                if ( env->threadRun[tNum] )
                    ++running;
                prgCur += env->threadPrg[tNum];
            }

            // Second display our progress.
            showMsg( env, "Rendering: %d%%", std::round( prgCurr * 100 / env->scrSize ) );

            // Third waste ms fetching events until 100 ms is hit
            if ( slept < 100 )
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 - slept ) );
        } // end of "we are waiting"

        // finally clear away our threads:
        env->clearThreads();

        if ( env->withGUI ) {
            // Clear Screen
            env->screen->clear();
            // Add image
            env->texture.loadFromImage( env->image );
            env->screen->draw( sf::Sprite( env->texture ) );
            // Display what we've drawn
            env->screen->display();

            // Finally show that we are ready:
            showMsg( env, "Ready!" );
        } else
            // Or save what we've got:
            result = saveTexture( env );
    } catch ( std::exception& e ) {
        cerr << "ERROR:  failed to create texture!" << endl;
        cerr << "REASON: " << e.what() << endl;
        result = EXIT_FAILURE;
    }

    return ( result );
}


int32_t swapImage( ENVIRONMENT* env ) {
    int32_t result = EXIT_SUCCESS;

    if ( env->withBump ) {
        // We have something to swap:
        if ( env->mainShown )
            // Switch to bumpmap
            env->texture.loadFromImage( env->bumpMap );
        else
            // Switch to texture
            env->texture.loadFromImage( env->image );

        env->screen->clear();
        env->screen->draw( sf::Sprite( env->texture ) );
        env->screen->display();

        // Tell env:
        env->mainShown = !env->mainShown;
    }

    return ( result );
}


/// @brief thread function for parallel drawing
void thrdDraw( void* xEnv ) {
    threadEnv*   thrdEnv = static_cast<threadEnv*>( xEnv );
    ENVIRONMENT* env     = thrdEnv->env;
    int32_t      tNum    = thrdEnv->threadNum;

    // Kick it!
    delete thrdEnv;

    // shortcuts
    int32_t      maxX      = env->scrWidth;
    int32_t      maxY      = env->scrHeight;
    double       rangeHigh = env->borderHi - env->borderMid;
    double       rangeLow  = env->borderMid - env->borderLo;
    double       rangeMid  = std::max( rangeLow, rangeHigh );
    double       reduct    = env->spxRedu;
    double       smooth    = env->spxSmoo;
    int32_t      waves     = env->spxWave;
    double       zoom      = env->spxZoom;
    // dynamic values
    double       xPortion  = std::round( maxX / static_cast<double>( env->numThreads ) ); // Number of X for this thread
    int32_t      xStart    = static_cast<int32_t>( xPortion * static_cast<double>( tNum ) ); // First x-coordinate this thread
    // handles. Generally the thread stops before reaching the unit number the next would start with. But the final thread
    // has to do the remaining numbers, which might be present due to rounding errors in the portion calculation
    int32_t      xStop     = tNum == ( env->numThreads - 1 ) ? maxX
                             : static_cast<int32_t>( xPortion * static_cast<double>( tNum + 1 ) );

    // coordinate modifiers
    double dX = env->offX, dY = env->offY, dZ = env->offZ, dW = env->offW;

    env->lock();
    env->threadPrg[tNum] = 0;
    env->threadRun[tNum] = true;
    env->unlock();

    // "y" is not necessarily the y-coordinate, but the smaller dimension of x and y
    for ( int32_t y = 0 ; y < maxY ; ++y ) {
        env->setCoordsByY( y, dY, dZ, dW );

        for ( int32_t x = xStart ; x < xStop ; ++x ) {
            double simplex;
            env->setCoordsByX( x, y, dX, dY, dZ, dW );

            RNG.lock();
            if ( env->dimensions == 4 )
                simplex = RNG.simplex4D( dX, dY, dZ, dW, zoom, smooth, reduct, waves );
            else if ( env->dimensions == 3 )
                simplex = RNG.simplex3D( dX, dY, dZ,     zoom, smooth, reduct, waves );
            else
                simplex = RNG.simplex2D( dX, dY,         zoom, smooth, reduct, waves );
            RNG.unlock();

            // Generate distance values according to the three borders:
            double distLo = simplex > env->borderLo
                            ? 1.0 - ( pwx::absDistance( env->borderLo,  0.0, simplex, 0.0 ) / rangeLow ) : 1.0;
            double distMi = 1.0 - ( pwx::absDistance( env->borderMid,  0.0, simplex, 0.0 ) / rangeMid );
            double distHi = simplex < env->borderHi
                            ? 1.0 - ( pwx::absDistance( env->borderHi,  0.0, simplex, 0.0 ) / rangeHigh ) : 1.0;


            // Get Color values:

            uint8_t cR = ( distLo > 0 ? static_cast<uint8_t>( std::round( distLo * env->colLow.r ) ) : 0 )
                         + ( distMi > 0 ? static_cast<uint8_t>( std::round( distMi * env->colMid.r ) ) : 0 )
                         + ( distHi > 0 ? static_cast<uint8_t>( std::round( distHi * env->colHi.r ) ) : 0 );
            uint8_t cG = ( distLo > 0 ? static_cast<uint8_t>( std::round( distLo * env->colLow.g ) ) : 0 )
                         + ( distMi > 0 ? static_cast<uint8_t>( std::round( distMi * env->colMid.g ) ) : 0 )
                         + ( distHi > 0 ? static_cast<uint8_t>( std::round( distHi * env->colHi.g ) ) : 0 );
            uint8_t cB = ( distLo > 0 ? static_cast<uint8_t>( std::round( distLo * env->colLow.b ) ) : 0 )
                         + ( distMi > 0 ? static_cast<uint8_t>( std::round( distMi * env->colMid.b ) ) : 0 )
                         + ( distHi > 0 ? static_cast<uint8_t>( std::round( distHi * env->colHi.b ) ) : 0 );

            // Now put the pixel onto the image
            env->image.setPixel( x, y, sf::Color( cR, cG, cB, 255 ) );

            // Generate bumpmap pixel if wanted:
            if ( env->withBump ) {
                double distGray = 1.0 - ( 0.6715 * pwx::absDistance( 0.33, -0.33, simplex, simplex ) );
                uint8_t cGray = static_cast<uint8_t>( std::round(
                        distGray > 0.0 ? distGray * ( static_cast<double>( cR + cG + cB ) / 3.0 ) : 0.0 ) );
                env->bumpMap.setPixel( x, y, sf::Color( cGray, cGray, cGray, 255 ) );
            }
        } // End of inner loop
    } // end of outer loop

    // Tell env that we are finished:
    env->lock();
    env->threadRun[tNum] = false;
    env->unlock();
}

