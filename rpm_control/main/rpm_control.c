#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../components/base_nav/base_nav.h"
#include "../components/pin_defs_for_single_motor/pin_defs_for_single_motor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

// int pwm = 0;
// int motor_velocity;

motor_commander_t motor_0 = (motor_commander_t) {.id = 0, .curr_velocity = 0, .prev_velocity = 0, .desr_velocity = 0, .dir_0_pin = MOTOR_0_A, .dir_1_pin = MOTOR_0_B, .pwm.pwm_unit = MCPWM_UNIT_0, .pwm.pwm_operator = MCPWM0A, .pwm.pwm_timer = MCPWM_TIMER_0, .pwm.pwm_pin = MOTOR_0_PWM };

static volatile int ticks_count_0 = 0;

static void IRAM_ATTR gpio_isr_handler_0(void* arg){
    if(gpio_get_level(ENCODER_PHASE_B_0) == 0)
    {
        ticks_count_0++;
    }
    else 
    {
        ticks_count_0--;
    }
}

static void print_ticks(void* arg){
    while(true)
    {
        printf("The Encoder ticks for motor 0: %d, \tvel: %d,\tpwm: %d\n", ticks_count_0, motor_velocity, pwm);
    }
}

static void calculate_velocity(void* arg){
    // int64_t motor_velocity_local = (int64_t) arg;
    motor_0.curr_velocity = ticks_count_0 * 4;    //(1000000*60)/(135*111111) = 4.0000400004
    ticks_count_0 = 0;
}

static void setup_velocity_calculator(void* arg){
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = calculate_velocity,
            // .arg = (void*) &motor_velocity,
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, 111111);
    vTaskDelete(NULL);
}

void app_main()
{ 
    init_gpio(ENCODER_PHASE_B_0,GPIO_MODE_INPUT);
    init_interrupt(ENCODER_PHASE_A_0);
	gpio_isr_handler_add(ENCODER_PHASE_A_0, gpio_isr_handler_0, (void*) ENCODER_PHASE_B_0);
	motor_0.desr_velocity = ;
    xTaskCreate(print_ticks, "print_ticks", 2048, NULL, 9, NULL);
     
    xTaskCreate(drive_motor, "drive_motor", 4096, &motor_0, 11, NULL);
    xTaskCreate(setup_velocity_calculator, "setup_velocity_calculator", 4096, NULL, 11, NULL); 
    while(1) 
    {
        // printf("gpio = %d\t%d\n", gpio_get_level(ENCODER_PHASE_A_0), gpio_get_level(ENCODER_PHASE_B_0));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
