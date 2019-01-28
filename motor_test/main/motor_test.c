#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../components/base_nav/base_nav.h"
#include "../components/pin_defs_for_single_motor/pin_defs_for_single_motor.h"


int pwm = 20;

// static volatile bool dir_0 = 0;
static volatile int16_t ticks_count_0 = 0;

static void IRAM_ATTR gpio_isr_handler_0(void* arg)
{
    if(gpio_get_level(ENCODER_PHASE_B_0) == 0)
    {
        ticks_count_0++;
    }
    else 
    {
        ticks_count_0--;
    }
}



static void print_ticks(void* arg)
{
    while(true)
    {
        printf("The Encoder ticks for motor 0: %d\t pwm: %d\n", ticks_count_0, pwm);
    }
}

// static void drive_motor(void* pwm)
static void drive_motor()
{
    int del = 1;
    motor_init();
    while(1)
    {
        vTaskDelay(100 / portTICK_RATE_MS);
        pwm += del;
        switch (pwm)
        {
            case -99:
                del = 1;
                break;
            case 99:
                del = -1;
                break;
            default:
                break;
        }

        if(pwm < 0){
            gpio_set_level(MOTOR_DIRECTION_A_0, 0);
            gpio_set_level(MOTOR_DIRECTION_B_0, 1);
        }
        else if(pwm > 0){
            gpio_set_level(MOTOR_DIRECTION_A_0, 1);
            gpio_set_level(MOTOR_DIRECTION_B_0, 0);
        }
        else{
            gpio_set_level(MOTOR_DIRECTION_A_0, 1);
            gpio_set_level(MOTOR_DIRECTION_B_0, 1);
        }
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, abs(pwm));
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
        // printf("In Drive_motor()\n");
    }
}


void app_main()
{
    config_input(ENCODER_PHASE_B_0);
    config_isr(ENCODER_PHASE_A_0);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ENCODER_PHASE_A_0, gpio_isr_handler_0, (void*) ENCODER_PHASE_B_0);
    
    xTaskCreate(print_ticks, "print_ticks", 2048, NULL, 10, NULL);
    // xTaskCreate(drive_motor, "drive_motor", 2048, (void*)&pwm_0, 11, NULL);    
    xTaskCreate(drive_motor, "drive_motor", 4096, NULL, 11, NULL);    
    while(1) 
    {
        // printf("gpio = %d\t%d\n", gpio_get_level(ENCODER_PHASE_A_0), gpio_get_level(ENCODER_PHASE_B_0));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}