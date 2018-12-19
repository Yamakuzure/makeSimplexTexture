#ifndef PWX_MAKESIMPLEXTEXTURE_SFMLUI_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_SFMLUI_H_INCLUDED 1
#pragma once

/** @file sfmlui.h
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


// This header adds some methods that are used by the workLoop only


#include <pwx_compiler.h>


#include "main.h"

int32_t saveTexture( ENVIRONMENT* env ) PWX_WARNUNUSED;
void showMsg( ENVIRONMENT* env, const char* fmt, ... );
int32_t showOSHelp( ENVIRONMENT* env ) PWX_WARNUNUSED;
int32_t showOSStat( ENVIRONMENT* env ) PWX_WARNUNUSED;
void showSeqPat( ENVIRONMENT* env, char* seqZtgt = NULL, char* seqWtgt = NULL );
void showState( ENVIRONMENT* env );

#endif // PWX_MAKESIMPLEXTEXTURE_SFMLUI_H_INCLUDED
