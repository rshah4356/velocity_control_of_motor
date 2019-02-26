#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "init_sra.h"
#include "pin_defs.h"
#include "motor_commander.h"

// encoder_commander_t encoder_0 = (encoder_commander_t) {.name = "ENCODER_0", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_0_A, .enc_dir = ENCODER_0_B};
// mcpwm_t pwm_A = (mcpwm_t) {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0A, .pwm_pin = MOTOR_0_PWM_A};
// mcpwm_t pwm_B = (mcpwm_t) {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0B, .pwm_pin = MOTOR_0_PWM_B};
motor_commander_t motor_0 = (motor_commander_t) {.name = "MOTOR_0", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75,  \
                        .encoder = {.name = "MOTOR_0", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_0_A, .enc_dir = ENCODER_0_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0A, .pwm_pin = MOTOR_0_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0B, .pwm_pin = MOTOR_0_PWM_B}  \
                    };

void app_main(){
    init_motor(&motor_0);
    init_gpio(SWITCH_MODE, GPIO_MODE_INPUT);
    init_gpio(SWITCH_INC, GPIO_MODE_INPUT);
    init_gpio(SWITCH_DEC, GPIO_MODE_INPUT);
    xTaskCreate(drive_motor, "drive_motor", 8192, &motor_0, 23, NULL);
    xTaskCreate(print_motor_status, "print_motor_status", 8192, &motor_0, 11, NULL);
    // xTaskCreate(tune_pid, "tune_pid", 8192, &motor_0, 11, NULL);
    while(true){
        vTaskDelay(40 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}