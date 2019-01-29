#ifndef _BASE_NAV_H
#define _BASE_NAV_H

#include "driver/gpio.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "esp_timer.h"
#include "../pin_defs_for_single_motor/pin_defs_for_single_motor.h"

#define ESP_INTR_FLAG_DEFAULT 0
#ifndef KP
    #define KP 0.05
#endif
#ifndef KD
    #define KD 0.0
#endif
#ifndef KI
    #define KI 0.0
#endif

typedef struct{
    mcpwm_unit_t pwm_unit;
    mcpwm_io_signals_t pwm_operator;
    mcpwm_timer_t pwm_timer;
    gpio_num_t pwm_pin;
}motor_pwm_t;

typedef struct{
    char name[10];
    int id;    
    int curr_velocity;
    int desr_velocity;
    gpio_num_t dir_0_pin;
    gpio_num_t dir_1_pin;
    motor_pwm_t pwm;
    int duty_cycle;
    int prev_duty_cycle;
    int delta_duty_cycle;
}motor_commander_t;



void init_interrupt(gpio_num_t NUM_GPIO);

void init_gpio(gpio_num_t NUM_GPIO, gpio_mode_t GPIO_MODE);

void init_pwm(motor_pwm_t *motor);

void init_motor(motor_commander_t *motor);

void print_motor_status(motor_commander_t *motor);

void drive_motor(motor_commander_t *motor);

#endif