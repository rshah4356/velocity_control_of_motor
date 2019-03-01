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
motor_commander_t motor_F = (motor_commander_t) {.name = "MOTOR_F", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_F", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_F_A, .enc_dir = ENCODER_F_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_F_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_F_PWM_B}  \
                    };

motor_commander_t motor_L = (motor_commander_t) {.name = "MOTOR_L", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_L", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_L_A, .enc_dir = ENCODER_L_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_L_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_0, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_L_PWM_B}  \
                    };

motor_commander_t motor_R = (motor_commander_t) {.name = "MOTOR_R", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0, \
                        .encoder = {.name = "MOTOR_R", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_R_A, .enc_dir = ENCODER_R_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM0A, .pwm_pin = MOTOR_R_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_0, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM0B, .pwm_pin = MOTOR_R_PWM_B}  \
                    };

motor_commander_t motor_B = (motor_commander_t) {.name = "MOTOR_B", .id = 0, .desr_rpm = 200, .Kp = 0.003,  .Kd = 0.75, .duty_cycle = 0,  \
                        .encoder = {.name = "MOTOR_B", .id = 0, .curr_rpm = 0, .ticks_count = 0, .enc_intr = ENCODER_B_A, .enc_dir = ENCODER_B_B},  \
                        .pwm_A = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_A, .pwm_io_signals = MCPWM1A, .pwm_pin = MOTOR_B_PWM_A}, \
                        .pwm_B = {.pwm_unit = MCPWM_UNIT_1, .pwm_timer = MCPWM_TIMER_1, .pwm_operator = MCPWM_OPR_B, .pwm_io_signals = MCPWM1B, .pwm_pin = MOTOR_B_PWM_B}  \
                    };
/*
*/

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

/*
void read_bot_motion_command_on_uart_without_padding_without_mapping(uint8_t* data_buffer){
    uart_port_t uart_num = UART_NUM_1;
    while(true){
        int len = uart_read_bytes(uart_num, data_buffer, BUF_SIZE, 20 / portTICK_RATE_MS);
        if((0 == *data_buffer)){ //0
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else if((50 > *data_buffer)){    // 1 - 49
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 25;
            motor_R.duty_cycle = 25;
        }
        else if((50 == *data_buffer)){   //50
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else if((100 > *data_buffer)){   //51 - 99
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = -25;
            motor_R.duty_cycle = -25;
        }
        else if((100 == *data_buffer)){   //100
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else if((150 > *data_buffer)){   //101 - 149
            motor_F.duty_cycle = -25;
            motor_B.duty_cycle = -25;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else if((150 == *data_buffer)){   //150
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else if((200 > *data_buffer)){   //151 - 199
            motor_F.duty_cycle = 25;
            motor_B.duty_cycle = 25;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else{   //200 - 255
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        printf("uart_data_received: %d\n", *data_buffer);
        // printf("FFFFFF");
        write_duty_cycle(&motor_F);
        // printf("LLLLLL");
        // write_duty_cycle(&motor_L);
        // printf("BBBBBB");
        // write_duty_cycle(&motor_B);
        // printf("RRRRRR");
        // write_duty_cycle(&motor_R);
        // printf("OOOOOOOOOVVVVVVER");
        printf("motor duty cycles: %f\t%f\t%f\t%f\n", motor_F.duty_cycle, motor_B.duty_cycle, motor_L.duty_cycle, motor_R.duty_cycle);
        vTaskDelay(40 / portTICK_RATE_MS);
    }
}

void read_bot_motion_command_on_uart_with_buffer(uint8_t* data_buffer){
    uart_port_t uart_num = UART_NUM_1;
    while(true){
        int len = uart_read_bytes(uart_num, data_buffer, BUF_SIZE, 20 / portTICK_RATE_MS);
        int command = *data_buffer;
        int pwm = 0;
        if((command >= 1) && (command <= 99)){
            pwm = command - 50;
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = pwm;
            motor_R.duty_cycle = pwm;
        }
        else if((command >= 101) && (command <= 199)){
            pwm = command - 150;
            motor_F.duty_cycle = pwm;
            motor_B.duty_cycle = pwm;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else{
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        printf("uart_data_received: %d\n", *data_buffer);
        // printf("FFFFFF");
        // write_duty_cycle(&motor_F);
        // printf("LLLLLL");
        // write_duty_cycle(&motor_L);
        // printf("BBBBBB");
        // write_duty_cycle(&motor_B);
        // printf("RRRRRR");
        // write_duty_cycle(&motor_R);
        // printf("OOOOOOOOOVVVVVVER");
        printf("motor duty cycles: %f\t%f\t%f\t%f\n", motor_F.duty_cycle, motor_B.duty_cycle, motor_L.duty_cycle, motor_R.duty_cycle);
        vTaskDelay(40 / portTICK_RATE_MS);
    }
}
*/

void read_bot_motion_command_on_uart(uint8_t* data_buffer){
    uart_port_t uart_num = UART_NUM_1;
    while(true){
        uart_read_bytes(uart_num, data_buffer, BUF_SIZE, 20 / portTICK_RATE_MS);
        int command = *data_buffer;
        // switch (command)
        // {
        //     case 25:
        //         motor_F.duty_cycle = 0;
        //         motor_B.duty_cycle = 0;
        //         motor_L.duty_cycle = 65;
        //         motor_R.duty_cycle = 65;
        //         break;
        //     case 75:
        //         motor_F.duty_cycle = 0;
        //         motor_B.duty_cycle = 0;
        //         motor_L.duty_cycle = -65;
        //         motor_R.duty_cycle = -65;
        //         break;
        //     case 125:
        //         motor_F.duty_cycle = 65;
        //         motor_B.duty_cycle = 65;
        //         motor_L.duty_cycle = 0;
        //         motor_R.duty_cycle = 0;
        //         break;
        //     case 175:
        //         motor_F.duty_cycle = -65;
        //         motor_B.duty_cycle = -65;
        //         motor_L.duty_cycle = 0;
        //         motor_R.duty_cycle = 0;
        //         break;
        //     default:
        //         motor_F.duty_cycle = 0;
        //         motor_B.duty_cycle = 0;
        //         motor_L.duty_cycle = 0;
        //         motor_R.duty_cycle = 0;
        //         break;
        // }
        int pwm = 0;
        if((command >= 1) && (command <= 99)){
            pwm = 2*(command - 50);
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            if(command == 50){
                motor_L.duty_cycle = 0;
                motor_R.duty_cycle = 0;
            }
            else{
                motor_L.duty_cycle = pwm;
                motor_R.duty_cycle = -pwm;
            }
        }
        else if((command >= 101) && (command <= 199)){
            pwm = 2* (command - 150);
            motor_F.duty_cycle = pwm;
            motor_B.duty_cycle = -pwm;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        else if((command >= 200) && (command <= 250)){
            pwm = 1.5*(command - 225);
            motor_F.duty_cycle = pwm;
            motor_B.duty_cycle = pwm;
            motor_L.duty_cycle = pwm;
            motor_R.duty_cycle = pwm;
        }
        else{
            motor_F.duty_cycle = 0;
            motor_B.duty_cycle = 0;
            motor_L.duty_cycle = 0;
            motor_R.duty_cycle = 0;
        }
        printf("uart_data_received: %d, command: %d, pwm %d\n", *data_buffer, command, pwm);
        printf("motor duty cycles: %f\t%f\t%f\t%f\n", motor_F.duty_cycle, motor_B.duty_cycle, motor_L.duty_cycle, motor_R.duty_cycle);
        // printf("FFFFFF");
        // motor_commander_t* motor_F_ptr = &motor_F;
        // write_duty_cycle(motor_F_ptr);
        // printf("LLLLLL");
        // motor_commander_t* motor_L_ptr = &motor_L;
        // write_duty_cycle(motor_L_ptr);
        // printf("BBBBBB");
        // motor_commander_t* motor_B_ptr = &motor_B;
        // write_duty_cycle(motor_B_ptr);
        // printf("RRRRRR");
        // motor_commander_t* motor_R_ptr = &motor_R;
        // write_duty_cycle(motor_R_ptr);


        // printf("OOOOOOOOOVVVVVVER");
        vTaskDelay(10 / portTICK_RATE_MS);
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
    init_uart(uart_config, UART_NUM_1);
    init_motor(&motor_F);
    init_motor(&motor_L);
    init_motor(&motor_B);
    init_motor(&motor_R);
    uint8_t *uart_rx_buffer = (uint8_t *) malloc(BUF_SIZE);
    // uint8_t *uart_tx_buffer = (uint8_t *) malloc(BUF_SIZE);
    xTaskCreate(read_bot_motion_command_on_uart, "tele_op_receiver", 8192, &uart_rx_buffer, 22, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor F", 8192, &motor_F, 23, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor L", 8192, &motor_L, 23, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor R", 8192, &motor_R, 23, NULL);
    xTaskCreate(write_duty_cycle_loop, "drive_motor B", 8192, &motor_B, 23, NULL);
    
    while(true){
        vTaskDelay(40 / portTICK_RATE_MS);
    }
    // vTaskDelete(NULL);
}