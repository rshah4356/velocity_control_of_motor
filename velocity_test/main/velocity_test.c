#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "freertos/queue.h"
// #include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "esp_timer.h"
#include "../../common_files/pin_defs_for_single_motor.h"

#define ESP_INTR_FLAG_DEFAULT 0

int pwm = 0;
int motor_velocity;

static volatile int16_t ticks_count_0 = 0;

static gpio_config_t io_conf;

static void config_isr(int arg){
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = ((uint64_t)1 << arg);
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

static void config_gpio(int arg, gpio_mode_t GPIO_MODE){
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.pin_bit_mask = ((uint64_t)1 << arg);
    io_conf.mode = GPIO_MODE;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);
}

static void IRAM_ATTR gpio_isr_handler_0(void* arg){
    if(gpio_get_level(ENCODER_PHASE_B_0) == 0){
        ticks_count_0++;
    }
    else{
        ticks_count_0--;
    }
}

static void motor_init(){
    gpio_pad_select_gpio(MOTOR_0_A);
    gpio_pad_select_gpio(MOTOR_0_B);
    gpio_set_direction(MOTOR_0_A, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_0_B, GPIO_MODE_OUTPUT);

    printf("initializing mcpwm\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MOTOR_0_PWM);
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    //frequency = 500Hz,
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxB = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}

static void print_ticks(void* arg){
    while(true){
        printf("The Encoder ticks for motor 0: %d, \tvel: %d,\tpwm: %d\n", ticks_count_0, motor_velocity, pwm);
    }
}

static void calculate_velocity(void* arg){
    // int64_t* motor_velocity_local = (int64_t*) arg;
    // *motor_velocity_local = ticks_count_0 * 4;    //(1000000*60)/(135*111111) = 4.0000400004
    motor_velocity = ticks_count_0 * 4;    //(1000000*60)/(135*111111) = 4.0000400004
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

// static void drive_motor(void* pwm)
static void drive_motor(){
    int del = 1;
    motor_init();
    while(1){
        vTaskDelay(100 / portTICK_RATE_MS);
        pwm += del;
        switch (pwm){
            case -100:
                del = 1;
                break;
            case 100:
                del = -1;
                break;
            default:;
        }

        if(pwm < 0){
            gpio_set_level(MOTOR_0_A, 0);
            gpio_set_level(MOTOR_0_B, 1);
        }
        else if(pwm > 0){
            gpio_set_level(MOTOR_0_A, 1);
            gpio_set_level(MOTOR_0_B, 0);
        }
        else{
            gpio_set_level(MOTOR_0_A, 1);
            gpio_set_level(MOTOR_0_B, 1);
        }
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, abs(pwm));
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
        // printf("In Drive_motor()\n");
    }
}

void app_main(){
    config_gpio(ENCODER_PHASE_B_0, GPIO_MODE_INPUT);
    config_isr(ENCODER_PHASE_A_0);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ENCODER_PHASE_A_0, gpio_isr_handler_0, NULL);
    
    xTaskCreate(print_ticks, "print_ticks", 2048, NULL, 9, NULL);
    // xTaskCreate(setup_velocity_calculator, "setup_velocity_calculator", 4096, NULL, 11, NULL); 
    xTaskCreate(drive_motor, "drive_motor", 4096, NULL, 11, NULL);
    xTaskCreate(setup_velocity_calculator, "setup_velocity_calculator", 4096, NULL, 11, NULL); 
    while(true){
        // printf("gpio = %d\t%d\n", gpio_get_level(ENCODER_PHASE_A_0), gpio_get_level(ENCODER_PHASE_B_0));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}