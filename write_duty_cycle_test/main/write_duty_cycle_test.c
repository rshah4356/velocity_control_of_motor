#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#include "init_sra.h"
#include "pin_defs.h"
#include "motor_commander.h"

// encoder_commander_t encoder_0 = (encoder_commander_t) {.name = "ENCODER_0", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_0_A, .enc_dir = ENCODER_0_B};
// mcpwm_t pwm_A = (mcpwm_t) {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0A, .pwm_pin = MOTOR_0_PWM_A};
// mcpwm_t pwm_B = (mcpwm_t) {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0B, .pwm_pin = MOTOR_0_PWM_B};
motor_commander_t motor_F_ = (motor_commander_t) {.name = "MOTOR_F", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_F", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_F_A, .enc_dir = ENCODER_F_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_F_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_F_PWM_B}  \
                    };

motor_commander_t motor_L_ = (motor_commander_t) {.name = "MOTOR_L", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_L", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_L_A, .enc_dir = ENCODER_L_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_L_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_L_PWM_B}  \
                    };

motor_commander_t motor_R_ = (motor_commander_t) {.name = "MOTOR_R", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0, \
                        .encoder = {.name = "MOTOR_R", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_R_A, .enc_dir = ENCODER_R_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_R_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_R_PWM_B}  \
                    };

motor_commander_t motor_B_ = (motor_commander_t) {.name = "MOTOR_B", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_B", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_B_A, .enc_dir = ENCODER_B_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_B_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_B_PWM_B}  \
                    };
/*
*/

void write_duty_cycle_loop(motor_commander_t* motor){
    while(1){

        // for(int i = 0; i < 100; i++){
            // vTaskDelay(10 / portTICK_RATE_MS);
            motor->duty_cycle = 25;
            write_duty_cycle(motor);
        // }
        // for(int i = 0; i < 100; i++){
            // vTaskDelay(10 / portTICK_RATE_MS);
            // motor->duty_cycle = i;
            // write_duty_cycle(motor);
        // }
        // for(int i = 100; i > 0; i--){
            // vTaskDelay(10 / portTICK_RATE_MS);
            // motor->duty_cycle = i;
            // write_duty_cycle(motor);
        // }
        // for(int i = 0; i < 100; i++){
            // vTaskDelay(10 / portTICK_RATE_MS);
            // motor->duty_cycle = 0;
            // write_duty_cycle(motor);
        // }
        // for(int i = 0; i > -100; i--){
            // vTaskDelay(10 / portTICK_RATE_MS);
            // motor->duty_cycle = i;
            // write_duty_cycle(motor);
        // }
        // for(int i = -100; i < 0; i++){
            // vTaskDelay(10 / portTICK_RATE_MS);
            // motor->duty_cycle = i;
            // write_duty_cycle(motor);
        // }
        // for(int i = 0; i < 100; i++){
            // vTaskDelay(10 / portTICK_RATE_MS);
            // motor->duty_cycle = 0;
            // write_duty_cycle(motor);
        // }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void app_main(){
    static motor_commander_t* motor_F;
    motor_F = &motor_F_;
    // static motor_commander_t* motor_B;
    // motor_B = &motor_B_;
    // static motor_commander_t* motor_L;
    // motor_L = &motor_L_;
    // static motor_commander_t* motor_R;
    // motor_R = &motor_R_;
    init_motor(motor_F);
    // init_motor(motor_L);
    // init_motor(motor_B);
    // init_motor(motor_R);
    xTaskCreate(write_duty_cycle_loop, "drive_motor F", 8192, motor_F, 23, NULL);
    // xTaskCreate(write_duty_cycle_loop, "drive_motor L", 8192, motor_L, 23, NULL);
    // xTaskCreate(write_duty_cycle_loop, "drive_motor R", 8192, motor_R, 23, NULL);
    // xTaskCreate(write_duty_cycle_loop, "drive_motor B", 8192, motor_B, 23, NULL);
    xTaskCreate(print_motor_status, "print_motor_status F", 2048, motor_F, 11, NULL);
    // xTaskCreate(print_motor_status, "print_motor_status L", 2048, motor_L, 11, NULL);
    // xTaskCreate(print_motor_status, "print_motor_status B", 2048, motor_B, 11, NULL);
    // xTaskCreate(print_motor_status, "print_motor_status R", 2048, motor_R, 11, NULL);
    while(true){
        vTaskDelay(40 / portTICK_RATE_MS);
    }
    // vTaskDelete(NULL);
}