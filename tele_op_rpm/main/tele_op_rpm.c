#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#include "init_sra.h"
#include "pin_defs.h"
#include "motor_commander.h"
#define MOTOR_KP 0.02
#define MOTOR_KD 1.269
motor_commander_t motor_F = (motor_commander_t) {.name = "MOTOR_F", .id = 0, .desr_rpm = 0, .Kp = MOTOR_KP,  .Kd = MOTOR_KD, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_F", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_F_A, .enc_dir = ENCODER_F_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_F_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_F_PWM_B}  \
                    };

motor_commander_t motor_L = (motor_commander_t) {.name = "MOTOR_L", .id = 0, .desr_rpm = 0, .Kp = MOTOR_KP,  .Kd = MOTOR_KD, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_L", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_L_A, .enc_dir = ENCODER_L_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_L_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_L_PWM_B}  \
                    };

motor_commander_t motor_R = (motor_commander_t) {.name = "MOTOR_R", .id = 0, .desr_rpm = 0, .Kp = MOTOR_KP,  .Kd = MOTOR_KD, .duty_cycle = 0, \
                        .encoder = {.name = "MOTOR_R", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_R_A, .enc_dir = ENCODER_R_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_R_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_R_PWM_B}  \
                    };

motor_commander_t motor_B = (motor_commander_t) {.name = "MOTOR_B", .id = 0, .desr_rpm = 0, .Kp = MOTOR_KP,  .Kd = MOTOR_KD, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_B", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_B_A, .enc_dir = ENCODER_B_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_B_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_B_PWM_B}  \
                    };

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

void read_bot_motion_command_on_uart(uint8_t* data_buffer){
    uart_port_t uart_num = UART_NUM_0;
    while(true){
        uart_read_bytes(uart_num, data_buffer, BUF_SIZE, 20 / portTICK_RATE_MS);
        int command = *data_buffer;
        int pwm = 0;
        if((command >= 1) && (command <= 99)){
            pwm = 6*(command - 50);
            motor_F.desr_rpm = 0;
            motor_B.desr_rpm = 0;
            if(command == 50){
                motor_L.desr_rpm = 0;
                motor_R.desr_rpm = 0;
            }
            else{
                motor_L.desr_rpm = pwm;
                motor_R.desr_rpm = -pwm;
            }
        }
        else if((command >= 101) && (command <= 199)){
            pwm = 8* (command - 150);
            motor_F.desr_rpm = pwm;
            motor_B.desr_rpm = -pwm;
            motor_L.desr_rpm = 0;
            motor_R.desr_rpm = 0;
        }
        else if((command >= 200) && (command <= 250)){
            pwm = 1.75*(command - 225);
            motor_F.desr_rpm = pwm;
            motor_B.desr_rpm = pwm;
            motor_L.desr_rpm = pwm;
            motor_R.desr_rpm = pwm;
        }
        else{
            motor_F.desr_rpm = 0;
            motor_B.desr_rpm = 0;
            motor_L.desr_rpm = 0;
            motor_R.desr_rpm = 0;
        }
        printf("uart_data_received: %d, command: %d, pwm %d\n", *data_buffer, command, pwm);
        printf("motor desr_rpm: %d\t%d\t%d\t%d\n", motor_F.desr_rpm, motor_B.desr_rpm, motor_L.desr_rpm, motor_R.desr_rpm);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void app_main(){
    init_uart(uart_config, UART_NUM_0);
    init_motor(&motor_F);
    init_motor(&motor_L);
    init_motor(&motor_B);
    init_motor(&motor_R);
    uint8_t *uart_rx_buffer = (uint8_t *) malloc(BUF_SIZE);
    // uint8_t *uart_tx_buffer = (uint8_t *) malloc(BUF_SIZE);
    xTaskCreate(read_bot_motion_command_on_uart, "tele_op_receiver", 8192, &uart_rx_buffer, 22, NULL);
    // xTaskCreate(drive_motor, "drive_motor", 2048, &motor_F, 23, NULL);
    xTaskCreate(drive_motor, "drive_motor", 2048, &motor_L, 23, NULL);
    // xTaskCreate(drive_motor, "drive_motor", 2048, &motor_B, 23, NULL);
    xTaskCreate(drive_motor, "drive_motor", 2048, &motor_R, 23, NULL);
    // xTaskCreate(print_motor_status, "print_motor_status", 2048, &motor_F, 11, NULL);
    xTaskCreate(print_motor_status, "print_motor_status", 2048, &motor_L, 11, NULL);
    // xTaskCreate(print_motor_status, "print_motor_status", 2048, &motor_B, 11, NULL);
    xTaskCreate(print_motor_status, "print_motor_status", 2048, &motor_R, 11, NULL);
    // xTaskCreate(tune_pid, "tune_pid", 8192, &motor_0, 11, NULL);
    while(true){
        vTaskDelay(40 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}