/*
 * MIT License
 *
 * Copyright (c) 2023 <#author_name#>
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

#include <stdlib.h>
#include <string.h>
#include "mnemonica_face.h"
#include <math.h>
#include <time.h>

void mnemonica_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void **context_ptr)
{
    if (*context_ptr == NULL)
    {
        *context_ptr = malloc(sizeof(mnemonica_state_t));
        memset(*context_ptr, 0, sizeof(mnemonica_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void mnemonica_face_activate(movement_settings_t *settings, void *context)
{
    mnemonica_state_t *state = (mnemonica_state_t *)context;
    // Use seconds to get card string
    state->cardIndex = floor(watch_rtc_get_date_time().unit.second);
    state->lightSwitch = 0;
    // Handle any tasks related to your watch face coming on screen.
}

static void _mnemonica_face_update_lcd(mnemonica_state_t *state)
{

    const char cardStack[][11] = {
        "4 ofClubs ",
        "2 ofHearts",
        "7 ofDimnds",
        "3 ofClubs ",
        "4 ofHearts",
        "6 ofDimnds",
        "A ofSpades",
        "5 ofHearts",
        "9 ofSpades",
        "2 ofSpades",
        "Q ofHearts",
        "3 ofDimnds",
        "Q ofClubs ",
        "8 ofHearts",
        "6 ofSpades",
        "5 ofSpades",
        "9 ofHearts",
        "K ofClubs ",
        "2 ofDimnds",
        "J ofHearts",
        "3 ofSpades",
        "8 ofSpades",
        "6 ofHearts",
        "10ofClubs ",
        "5 ofDimnds",
        "K ofDimnds",
        "2 ofClubs ",
        "3 ofHearts",
        "8 ofDimnds",
        "5 ofClubs ",
        "K ofSpades",
        "J ofDimnds",
        "8 ofClubs ",
        "10ofSpades",
        "K ofHearts",
        "J ofClubs ",
        "7 ofSpades",
        "10ofHearts",
        "A ofDimnds",
        "4 ofSpades",
        "7 ofHearts",
        "4 ofDimnds",
        "A ofClubs ",
        "9 ofClubs ",
        "J ofSpades",
        "Q ofDimnds",
        "7 ofClubs ",
        "Q ofSpades",
        "10ofDimnds",
        "6 ofClubs ",
        "A ofHearts",
        "9 ofDimnds"};

    char buf[11];

    if (state->cardIndex <= 1)
    {
        sprintf(buf, "%s", cardStack[0]);
    }
    else if (state->cardIndex >= 52)
    {
        sprintf(buf, "%s", cardStack[51]);
    }
    else
    {
        sprintf(buf, "%s", cardStack[state->cardIndex - 1]);
    }

    watch_display_string(buf, 0);
}

// static void _mnemonica_reveal_sound()
// {
//     movement_play_tune();
// }

bool mnemonica_face_loop(movement_event_t event, movement_settings_t *settings, void *context)
{
    mnemonica_state_t *state = (mnemonica_state_t *)context;

    switch (event.event_type)
    {
    case EVENT_ACTIVATE:
        // Show your initial UI here.
        _mnemonica_face_update_lcd(state);
        break;
    case EVENT_MODE_BUTTON_UP:
        movement_move_to_next_face();
        break;
    case EVENT_TICK:
        // If needed, update your display here.
        break;
    case EVENT_LIGHT_BUTTON_UP:
        // You can use the Light button for your own purposes. Note that by default, Movement will also
        // illuminate the LED in response to EVENT_LIGHT_BUTTON_DOWN; to suppress that behavior, add an
        // empty case for EVENT_LIGHT_BUTTON_DOWN.
        // If we're at the card index of 1 (4 of Clubs) we want to loop to 52 (9 of Diamonds)
        if (state->cardIndex == 1 || state->cardIndex >= 53)
        {
            state->cardIndex = 52;
        }
        else
        {
            state->cardIndex -= 1;
        }
        _mnemonica_face_update_lcd(state);

        // _mnemonica_reveal_sound();
        break;
    case EVENT_LIGHT_BUTTON_DOWN:
        break;
    case EVENT_LIGHT_LONG_PRESS:
        state->lightSwitch = !state->lightSwitch;
        if (state->lightSwitch)
        {
            watch_set_led_green();
        }
        else
        {
            watch_set_led_off();
        }
        break;
    case EVENT_ALARM_BUTTON_UP:
        // Just in case you have need for another button.
        if (state->cardIndex >= 52 || state->cardIndex <= 0)
        {
            state->cardIndex = 1;
        }
        else
        {
            state->cardIndex += 1;
        }
        _mnemonica_face_update_lcd(state);
        // _mnemonica_reveal_sound();
        break;
    case EVENT_ALARM_BUTTON_DOWN:
        break;
    case EVENT_TIMEOUT:
        // Your watch face will receive this event after a period of inactivity. If it makes sense to resign,
        // you may uncomment this line to move back to the first watch face in the list:
        movement_move_to_face(0);
        break;
    case EVENT_LOW_ENERGY_UPDATE:
        // If you did not resign in EVENT_TIMEOUT, you can use this event to update the display once a minute.
        // Avoid displaying fast-updating values like seconds, since the display won't update again for 60 seconds.
        // You should also consider starting the tick animation, to show the wearer that this is sleep mode:
        // watch_start_tick_animation(500);
        break;
    default:
        // Movement's default loop handler will step in for any cases you don't handle above:
        // * EVENT_LIGHT_BUTTON_DOWN lights the LED
        // * EVENT_MODE_BUTTON_UP moves to the next watch face in the list
        // * EVENT_MODE_LONG_PRESS returns to the first watch face (or skips to the secondary watch face, if configured)
        // You can override any of these behaviors by adding a case for these events to this switch statement.
        return movement_default_loop_handler(event, settings);
    }

    // return true if the watch can enter standby mode. Generally speaking, you should always return true.
    // Exceptions:
    //  * If you are displaying a color using the low-level watch_set_led_color function, you should return false.
    //  * If you are sounding the buzzer using the low-level watch_set_buzzer_on function, you should return false.
    // Note that if you are driving the LED or buzzer using Movement functions like movement_illuminate_led or
    // movement_play_alarm, you can still return true. This guidance only applies to the low-level watch_ functions.
    return true;
}

void mnemonica_face_resign(movement_settings_t *settings, void *context)
{
    (void)settings;
    (void)context;

    // handle any cleanup before your watch face goes off-screen.
}
