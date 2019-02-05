#ifndef _BASE_NAV_H
#define _BASE_NAV_H

#include "../init_sra/init_sra.h"
// #include "driver/gpio.h"
// #include "esp_attr.h"
// #include "driver/mcpwm.h"
// #include "soc/mcpwm_reg.h"
// #include "soc/mcpwm_struct.h"
// #include "esp_timer.h"
// #include "../pin_defs_for_single_motor/pin_defs_for_single_motor.h"



typedef struct{
    char  motor_name[10];
    int   id;    
    int   curr_velocity;
    int   desr_velocity;
    float Kp;
    float Kd;
    float Ki; 
    int duty_cycle;
    int del_duty_cycle;
    volatile long enc_ticks;
    gpio_num_t dir_0_pin;
    gpio_num_t dir_1_pin;
    mcpwm_t pwm;
}motor_commander_t;


void init_motor(motor_commander_t *motor);

void drive_motor(motor_commander_t *motor);

void IRAM_ATTR enc_isr_handler(gpio_num_t NUM_GPIO,motor_commander_t *motor);
#endif