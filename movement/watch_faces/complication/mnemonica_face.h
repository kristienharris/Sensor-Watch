/*
 * MIT License
 *
 * Copyright (c) 2023 Kristien Harris
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
 */

#ifndef MNEMONICA_FACE_H_
#define MNEMONICA_FACE_H_
#include "movement.h"

/*
 * Use mnemonica to display written playing cards on watch face
 *
 * Decide which card from mnemonica you want to display
 * Wait for the second counter to get to the index of the card in the stack
 * Press the mode button just before or on the second of the index number in seconds
 * The watch face will display a card if you are off by one either side of the index
 * just press the light button to go back in the stack and the alarm button to go forward in the stack
 *
 */

typedef struct
{
    // Anything you need to keep track of, put it here!
    uint8_t cardIndex;
    bool lightSwitch;
} mnemonica_state_t;

void mnemonica_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void **context_ptr);
void mnemonica_face_activate(movement_settings_t *settings, void *context);
bool mnemonica_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void mnemonica_face_resign(movement_settings_t *settings, void *context);

#define mnemonica_face ((const watch_face_t){ \
    mnemonica_face_setup,                     \
    mnemonica_face_activate,                  \
    mnemonica_face_loop,                      \
    mnemonica_face_resign,                    \
    NULL,                                     \
})

#endif // MNEMONICA_FACE_H_
