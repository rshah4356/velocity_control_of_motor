#ifndef _MOTOR_COMMANDER_H
#define _MOTOR_COMMANDER_H

#define KD_STEP 0.05
#define KP_STEP 0.002

#include "init_sra.h"
#include "encoder_commander.h"
// #include "driver/gpio.h"
// #include "esp_attr.h"
// #include "driver/mcpwm.h"
// #include "soc/mcpwm_reg.h"
// #include "soc/mcpwm_struct.h"
// #include "esp_timer.h"
// #include "../pin_defs_for_single_motor/pin_defs_for_single_motor.h"

typedef struct{
    char  name[10];
    int   id;    
    int   desr_rpm;
    int16_t err;
    int16_t prev_err;
    int16_t cum_err;
    float Kp;
    float Kd;
    bool tune_mode;
    float Ki; 
    float duty_cycle;
    float del_duty_cycle;
    mcpwm_t pwm_A;
    mcpwm_t pwm_B;
    encoder_commander_t encoder;
}motor_commander_t;

void init_motor(motor_commander_t *motor);

void calculate_duty_cycle(motor_commander_t *motor);

void write_duty_cycle(motor_commander_t *motor);

void drive_motor(motor_commander_t *motor);

void print_motor_status(motor_commander_t *motor);

// void tune_pid(motor_commander_t *motor);

#endif