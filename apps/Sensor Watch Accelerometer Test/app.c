#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "watch.h"
#include "lis2dh.h"

// This application displays data from the old Sensor Watch Motion sensor board.
// Note that this board required A0 to be set high to power the sensor.
// Future accelerometer boards will be powered directly from VCC.
// Also note that this board has its INT1 pin wired to A1, which is not an external
// wake pin. Future accelerometer boards will wire interrupt pins to A2 and A4.

void cb_light_pressed() {
}

void cb_mode_pressed() {
}

void cb_alarm_pressed() {
}

uint16_t interrupts = 0;
uint16_t ticks = 0;

void cb_interrupt_1() {
    interrupts++;
}

void cb_tick() {
    if (++ticks == 30) {
        interrupts = 0;
        ticks = 0;
        watch_display_string("IN t   0", 0);
    }
}

void app_init() {
    gpio_set_pin_direction(A0, GPIO_DIRECTION_OUT);
    gpio_set_pin_function(A0, GPIO_PIN_FUNCTION_OFF);
    gpio_set_pin_level(A0, true);

    watch_enable_display();
    watch_display_string("IN 0   0", 0);

    watch_enable_external_interrupts();
    watch_register_interrupt_callback(BTN_MODE, cb_mode_pressed, INTERRUPT_TRIGGER_RISING);
    watch_register_interrupt_callback(BTN_LIGHT, cb_light_pressed, INTERRUPT_TRIGGER_RISING);
    watch_register_interrupt_callback(BTN_ALARM, cb_alarm_pressed, INTERRUPT_TRIGGER_RISING);

    watch_enable_i2c();

    lis2dh_begin();
    lis2dh_set_data_rate(LIS2DH_DATA_RATE_10_HZ);
    lis2dh_configure_aoi_int1(
        LIS2DH_INTERRUPT_CONFIGURATION_OR |
        LIS2DH_INTERRUPT_CONFIGURATION_X_HIGH_ENABLE |
        LIS2DH_INTERRUPT_CONFIGURATION_Y_HIGH_ENABLE |
        LIS2DH_INTERRUPT_CONFIGURATION_Z_HIGH_ENABLE, 96, 0);

    watch_register_interrupt_callback(A1, cb_interrupt_1, INTERRUPT_TRIGGER_RISING);
    watch_rtc_register_tick_callback(cb_tick);
}

void app_wake_from_backup() {
}

void app_setup() {
}

void app_prepare_for_standby() {
}

void app_wake_from_standby() {
}

bool app_loop() {
    char buf[13] = {0};

    sprintf(buf, "IN%2d%4d", ticks, interrupts);
    watch_display_string(buf, 0);

    return true;
}
