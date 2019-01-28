#ifndef _BASE_NAV_H
#define _BASE_NAV_H

#include "driver/gpio.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "esp_timer.h"

#define ESP_INTR_FLAG_DEFAULT 0

#define KP 0.0
#define KD 0.0
#define KI 0.0

typedef struct{
    mcpwm_unit_t pwm_unit;
    mcpwm_io_signals_t pwm_operator;
    mcpwm_timer_t pwm_timer;
    gpio_num_t pwm_pin;
}motor_pwm_t;

typedef struct{
    int id;    
    int curr_velocity;
    int prev_velocity;
    int desr_velocity;
    gpio_num_t dir_0_pin;
    gpio_num_t dir_1_pin;
    motor_pwm_t pwm_pin;
}motor_commander_t;

void init_interrupt(gpio_num_t NUM_GPIO);

void init_gpio(gpio_num_t NUM_GPIO, gpio_mode_t GPIO_MODE);

void init_pwm(motor_pwm_t *motor);

void init_motor(motor_commander_t *motor);

void drive_motor(motor_commander_t *motor);

#endif