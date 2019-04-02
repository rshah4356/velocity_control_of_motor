#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#include "init_sra.h"
#include "pin_defs.h"
#include "motor_commander.h"

#define MOTOR_KP_STEP 0.005
#define MOTOR_KD_STEP 1.0
#define MAXPWM 65.0
#define MINPWM 35.0
#define ENCODING_FACTOR (MAXPWM)/40.0
// #define MULTIPLYING_FACTOR  
// encoder_commander_t encoder_0 = (encoder_commander_t) {.name = "ENCODER_0", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_0_A, .enc_dir = ENCODER_0_B};
// mcpwm_t pwm_A = (mcpwm_t) {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0A, .pwm_pin = MOTOR_0_PWM_A};
// mcpwm_t pwm_B = (mcpwm_t) {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM0B, .pwm_pin = MOTOR_0_PWM_B};
motor_commander_t motor_F = (motor_commander_t) {.name = "MOTOR_F", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_F", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_F_A, .enc_dir = ENCODER_F_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_F_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_F_PWM_B}  \
                    };

motor_commander_t motor_L = (motor_commander_t) {.name = "MOTOR_L", .id = 1, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_L", .id = 1, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_L_A, .enc_dir = ENCODER_L_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_L_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_L_PWM_B}  \
                    };

motor_commander_t motor_R = (motor_commander_t) {.name = "MOTOR_R", .id = 2, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0, \
                        .encoder = {.name = "MOTOR_R", .id = 2, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_R_A, .enc_dir = ENCODER_R_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_R_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_R_PWM_B}  \
                    };

motor_commander_t motor_B = (motor_commander_t) {.name = "MOTOR_B", .id = 3, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_B", .id = 3, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_B_A, .enc_dir = ENCODER_B_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_B_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_B_PWM_B}  \
                    };
/*
*/

uart_port_t esp_to_ros = UART_NUM_2;

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};
void read_bot_motion_command_on_uart(uint8_t* data_buffer){
    // int command_l = 0;
    // int command_r = 0;
    float pwm_l = 0;
    float pwm_r = 0;
    while(true){
        int len = uart_read_bytes(esp_to_ros, data_buffer, BUF_SIZE, 20 / portTICK_RATE_MS);
        int buffer_offset = 0;
        while(len--){
            int command = *(data_buffer + buffer_offset);
            if(command == 100){
                motor_L.Kp -= MOTOR_KP_STEP;
                motor_R.Kp -= MOTOR_KP_STEP;
                printf("KP:%lf\tKD:%lf\t", motor_R.Kp, motor_R.Kd);
                printf("motor desr_rpm: %d\t%d\t%d\t%d\n", motor_F.desr_rpm, motor_B.desr_rpm, motor_L.desr_rpm, motor_R.desr_rpm);
            }
            else if(command == 101){
                motor_L.Kp += MOTOR_KP_STEP;
                motor_R.Kp += MOTOR_KP_STEP;
                printf("KP:%lf\tKD:%lf\t", motor_R.Kp, motor_R.Kd);
                printf("motor desr_rpm: %d\t%d\t%d\t%d\n", motor_F.desr_rpm, motor_B.desr_rpm, motor_L.desr_rpm, motor_R.desr_rpm);
            }
            else if(command == 102){
                motor_L.Kd -= MOTOR_KD_STEP;
                motor_R.Kd -= MOTOR_KD_STEP;
                printf("KP:%lf\tKD:%lf\t", motor_R.Kp, motor_R.Kd);
                printf("motor desr_rpm: %d\t%d\t%d\t%d\n", motor_F.desr_rpm, motor_B.desr_rpm, motor_L.desr_rpm, motor_R.desr_rpm);
            }
            else if(command == 103){
                motor_L.Kd += MOTOR_KD_STEP;
                motor_R.Kd += MOTOR_KD_STEP;
                printf("KPs:%lf\tKD:%lf\t", motor_R.Kp, motor_R.Kd);
                printf("motor desr_rpm: %d\t%d\t%d\t%d\n", motor_F.desr_rpm, motor_B.desr_rpm, motor_L.desr_rpm, motor_R.desr_rpm);
            }
            else if(command == 105){
                printf("KP:%lf\tKD:%lf\t", motor_R.Kp, motor_R.Kd);
                printf("motor desr_rpm: %d\t%d\t%d\t%d\n", motor_F.desr_rpm, motor_B.desr_rpm, motor_L.desr_rpm, motor_R.desr_rpm);
            }
            else if(command & 128){
                // command_r = command & 127; 
                pwm_r = ENCODING_FACTOR * (command - 169);
                motor_F.duty_cycle = 0;
                motor_B.duty_cycle = 0;
                // motor_L.des = pwm;
                // if (pwm_r == 0)
                    // motor_R.duty_cycle = 0;
                // else if (pwm_r > 0)
                motor_R.duty_cycle =  pwm_r;
                // else 
                    // motor_R.duty_cycle = -MINPWM + pwm_r;

            }
            else{
                // command_l = command ;
                pwm_l = ENCODING_FACTOR * (command - 41);
                motor_F.duty_cycle = 0;
                motor_B.duty_cycle = 0;
                // if (pwm_l == 0)
                    // motor_L.duty_cycle = 0;
                // else if (pwm_l > 0)
                    motor_L.duty_cycle =  pwm_l;
                // else 
                    // motor_L.duty_cycle = -MINPWM + pwm_l;

            }
            printf("uart_data_received: %d, command_l: %f, command_r: %f\n", command, pwm_l, pwm_r);
            // printf("KP:%lf\tKD:%lf\t", motor_R.Kp, motor_R.Kd);
            // printf("motor duty_cycle: %f\t%f\t%f\t%f\n", motor_F.duty_cycle, motor_B.duty_cycle, motor_L.duty_cycle, motor_R.duty_cycle);
            vTaskDelay(10 / portTICK_RATE_MS);
            buffer_offset++;
        }
    }
}


void write_duty_cycle_loop(motor_commander_t* motor){
    while(1){
        // calculate_duty_cycle(motor);
        // printf("%s\tdc: %f\t\n", motor->name, motor->duty_cycle);
        write_duty_cycle(motor);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void app_main(){
    init_uart(uart_config, esp_to_ros);
    init_motor(&motor_F);
    init_motor(&motor_L);
    init_motor(&motor_B);
    init_motor(&motor_R);
    uint8_t *uart_rx_buffer = (uint8_t *) malloc(BUF_SIZE);
    uint8_t *uart_tx_buffer = (uint8_t *) malloc(BUF_SIZE);
    xTaskCreate(read_bot_motion_command_on_uart, "tele_op_receiver", 8192, &uart_rx_buffer, 22, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor F", 8192, &motor_F, 23, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor L", 8192, &motor_L, 23, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor R", 8192, &motor_R, 23, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor B", 8192, &motor_B, 23, NULL);
    // xTaskCreate(print_motor_status, "print_motor R", 8192, &motor_R, 20, NULL);
    // xTaskCreate(print_motor_status, "print_motor L", 8192, &motor_L, 20, NULL); 
    uint8_t ticks_L, ticks_R;
    while(true){
        ticks_L = (uint8_t)motor_L.encoder.total_ticks;
        *uart_tx_buffer = ticks_L + 41;

        // uart_write_bytes(esp_to_ros, (const char *)uart_tx_buffer, 1);
        uart_write_bytes(esp_to_ros, (const char *)uart_tx_buffer, 1);

        ticks_R = (uint8_t)motor_R.encoder.total_ticks;
        ticks_R = ticks_R + 41;
        ticks_R |= 128;
        *uart_tx_buffer = ticks_R; 
        uart_write_bytes(esp_to_ros, (const char *)uart_tx_buffer, 1);

        motor_L.encoder.total_ticks = 0;
        motor_R.encoder.total_ticks = 0;
        vTaskDelay(100 / portTICK_RATE_MS);
    }
    // vTaskDelete(NULL);
}