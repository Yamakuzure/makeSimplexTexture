/** @file workloop.cpp
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
#include <thread>

#include "workloop.h"


int32_t workLoop( ENVIRONMENT* env ) {
    int32_t   result  = EXIT_SUCCESS;
    bool      done    = false;
    bool      render  = false;
    int32_t   elapsed = 0;
    sf::Event event;
    sf::Clock clock;
    sf::Event::KeyEvent key;

    while ( !done && ( EXIT_SUCCESS == result ) && env->screen->isOpen() ) {
        /* ======================== *
         * === A) Proces events === *
         * ======================== */
        // message processing loop
        while ( env->screen->pollEvent( event ) ) {
            /// 1.: Check whether to quit:
            if ( sf::Event::Closed == event.type ) {
                env->screen->close();
                done = true;
            }

            /// 2.: Check Key presses:
            else if ( sf::Event::KeyPressed == event.type ) {
                if ( env->helpBoxShown ) {
                    // This only switches back to the image:
                    env->helpBoxShown = false;
                    env->removeBox    = true;
                } else if ( env->statBoxShown ) {
                    // This only switches back to the image:
                    env->statBoxShown = false;
                    env->removeBox    = true;
                } else {
                    // Otherwise process key presses:
                    key = event.key;
                    switch ( key.code ) {
                        case sf::Keyboard::Left:
                            env->offX -= key.control ? 10.0 : 1.0;
                            showMsg( env, "New Offset X: %g", env->offX );
                            break;
                        case sf::Keyboard::Right:
                            env->offX += key.control ? 10.0 : 1.0;
                            showMsg( env, "New Offset X: %g", env->offX );
                            break;
                        case sf::Keyboard::Down:
                            env->offY -= key.control ? 10.0 : 1.0;
                            showMsg( env, "New Offset Y: %g", env->offY );
                            break;
                        case sf::Keyboard::Up:
                            env->offY += key.control ? 10.0 : 1.0;
                            showMsg( env, "New Offset Y: %g", env->offY );
                            break;
                        case sf::Keyboard::Escape:
                            env->screen->close();
                            done = true;
                            break;
                        case sf::Keyboard::H:
                            result = showOSHelp( env );
                            break;
                        case sf::Keyboard::S:
                            result = saveTexture( env );
                            break;
                        case sf::Keyboard::Space:
                            result = swapImage( env );
                            break;
                        case sf::Keyboard::Tab:
                            result = showOSStat( env );
                            break;
                        case sf::Keyboard::R:
                            if ( key.shift )
                                render = true;
                            break;
                        case sf::Keyboard::W:
                            if ( key.alt ) {
                                // Change sequence pattern
                                if ( key.shift )
                                    env->seqW--;
                                else
                                    env->seqW++;
                                showSeqPat( env );
                            } // end of changing sequence
                            else {
                                // change offset
                                if ( key.shift )
                                    env->offW -= key.control ? 10.0 * env->modW : env->modW;
                                else
                                    env->offW += key.control ? 10.0 * env->modW : env->modW;
                                showMsg( env, "New Offset W: %g", env->offW );
                            } // end of changing offset
                            break;
                        case sf::Keyboard::Z:
                            if ( key.alt ) {
                                // Change sequence pattern
                                if ( key.shift ) {
                                    env->seqZ--;
                                    while ( ( env->dimensions < 4 )
                                            &&( ( env->seqZ == eiZincW )
                                                ||( env->seqZ == eiZisW )
                                                ||( env->seqZ == eiZisXYdW )
                                                ||( env->seqZ == eiZisXYmW )
                                              ) )
                                        env->seqZ--; // Skip sequencers that need a dimension not set
                                } else {
                                    env->seqZ++;
                                    while ( ( env->dimensions < 4 )
                                            &&( ( env->seqZ == eiZincW )
                                                ||( env->seqZ == eiZisW )
                                                ||( env->seqZ == eiZisXYdW )
                                                ||( env->seqZ == eiZisXYmW )
                                              ) )
                                        env->seqZ++; // Skip sequencers that need a dimension not set
                                }
                                showSeqPat( env );
                            } // End of changing sequence
                            else {
                                // change offset
                                if ( key.shift )
                                    env->offZ -= key.control ? 10.0 * env->modZ : env->modZ;
                                else
                                    env->offZ += key.control ? 10.0 * env->modZ : env->modZ;
                                showMsg( env, "New Offset Z: %g", env->offZ );
                            } // End of changing offset
                            break;
                        case sf::Keyboard::D:
                            if ( key.shift )
                                env->dimensions -= env->dimensions > 2 ? 1 : 0;
                            else
                                env->dimensions += env->dimensions < 4 ? 1 : 0;
                            showMsg( env, "New Number of dimensions: %d", env->dimensions );
                            break;
                        default:
                            // nothing....
                            break;
                    } // end of switching the key code
                } // end of nothing shown
            }
        } // end of message processing
        // Have we set a message?
        if ( env->msgShown < 0 ) {
            // Yes...
            env->msgShown = 1;
            clock.restart();
            elapsed = 0;
        }
        // We have to reset the elapsed time if we show the help or stat box,
        // those should not disappear automatically:
        else if ( ( elapsed > 5 )
                  && ( env->helpBoxShown || env->statBoxShown ) ) {
            elapsed = 0;
            clock.restart();
        }
        // Otherwise pick up the time:
        else
            elapsed = static_cast<int32_t>( clock.getElapsedTime().asMilliseconds() );

        /* ======================== *
         * === B) draw screen   === *
         * ======================== */
        if ( EXIT_SUCCESS == result && !done
                && ( ( elapsed > 5 && env->msgShown ) || env->helpBoxShown || env->statBoxShown || env->removeBox ) ) {

            env->texture.loadFromImage( env->image );
            env->screen->clear();
            // The textboxes are drawn over the image
            env->screen->draw( sf::Sprite( env->texture ) );
            if ( env->helpBoxShown ) {
                sf::Texture boxTex;
                boxTex.loadFromImage( *( env->helpText->txtBox ) );
                sf::Sprite box( boxTex, sf::IntRect( 5, 5, env->scrWidth - 5, env->scrHeight - 5 ) );
                env->screen->draw( box );
                for ( int i = 0; i < env->helpText->txtSize; ++i )
                    env->screen->draw( env->helpText->txtContent[i] );
            } else if ( env->statBoxShown ) {
                sf::Texture boxTex;
                boxTex.loadFromImage( *( env->statsText->txtBox ) );
                sf::Sprite box( boxTex, sf::IntRect( 5, 5, env->scrWidth - 5, env->scrHeight - 5 ) );
                env->screen->draw( box );
                for ( int i = 0; i < env->statsText->txtSize; ++i )
                    env->screen->draw( env->statsText->txtContent[i] );
            } else {
                // we have a reset.
                env->msgShown  = 0;
                env->removeBox = false;
            }
            // finally, update the screen
            env->screen->display();
        }

        /* ======================== *
         * === B) render image  === *
         * ======================== */
        if ( render && !done )
            result = loadNewImage( env );

        /* ======================== *
         * === C) waste time    === *
         * ======================== */
        if ( !done ) {
            if ( render )
                render = false;
            else
                // I daresay ten polls per second should be enough.
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 - elapsed ) );
        }
    } // end main loop

    return ( result );
}
