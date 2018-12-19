/** @file environment.cpp
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

#include "environment.h"

/** @brief return version as c-string
  *
  * @todo: document this function
  */
const char* ENVIRONMENT::getVersion() const {
    return( version.c_str() );
}

/** @brief Default destructor **/
ENVIRONMENT::~ENVIRONMENT() {
    // Be dead safe about our threads _first_!
    if ( thread ) {
        clearThreads();
        for ( int32_t tNum = 0; tNum < numThreads; ++tNum ) {
            if ( thread[tNum] ) {
                delete thread[tNum];
                thread[tNum] = NULL;
            }
        }
        delete [] thread;
    }
    if ( threadPrg )  delete [] threadPrg;
    if ( threadRun )  delete [] threadRun;
    if ( helpText )   delete helpText;
    if ( statsText )  delete statsText;
    if ( screen )     delete screen;
    if ( font )       delete font;
}

/// @brief simple function that finishes and clears all threads
void ENVIRONMENT::clearThreads() {
    for ( int32_t tNum = 0; tNum < numThreads; ++tNum ) {
        if ( threadRun[tNum] && thread[tNum] )
            thread[tNum]->wait();

        threadRun[tNum] = false;

        if ( thread[tNum] ) {
            delete thread[tNum];
            thread[tNum] = NULL;
        }
    }
}
