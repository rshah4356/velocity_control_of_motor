#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "freertos/queue.h"
// #include "freertos/event_groups.h"

#include "driver/gpio.h"
#include "esp_attr.h"


#define ENCODER_PHASE_A_0     16
#define ENCODER_PHASE_B_0     5
// #define MOTOR_DIRECTION_A_0   25
// #define MOTOR_DIRECTION_B_0   26
// #define MOTOR_PWM_0           27

#define ESP_INTR_FLAG_DEFAULT 0

// #define PWM_DUTY_0 20

// float pwm_0 = PWM_DUTY_0;

// static volatile bool dir_0 = 0;
static volatile int16_t count_0 = 0;

// static const char *TAG = "Rhino Robot";

// static xQueueHandle gpio_evt_queue_0 = NULL;

static gpio_config_t io_conf;

static void config_isr(int arg)
{
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = ((uint64_t)1 << arg);
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

static void config_input(int arg)
{
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.pin_bit_mask = ((uint64_t)1 << arg);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);
}


static void IRAM_ATTR gpio_isr_handler_0(void* arg)
{
    // count_0++;
    if(gpio_get_level(ENCODER_PHASE_B_0) == 0)
    {
        count_0++;
    }
    else 
    {
        count_0--;
    }

    // if(abs(count_0) == 100)
    // {
    //     pwm_0 = 0;
    // }
}

static void run_rhino(void* arg)
{
    for(;;) 
    {

        printf("The Encoder ticks for motor 0: %d\n", count_0);

    }
}

void app_main()
{
    
    config_isr(ENCODER_PHASE_A_0);
    
    config_input(ENCODER_PHASE_B_0);
    
    xTaskCreate(run_rhino, "rhino robot", 2048, NULL, 10, NULL);
    
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    gpio_isr_handler_add(ENCODER_PHASE_A_0, gpio_isr_handler_0, (void*) ENCODER_PHASE_B_0);
        
    while(1) 
    {
        // printf("while");
        // printf("gpio = %d\t%d\n", gpio_get_level(ENCODER_PHASE_A_0), gpio_get_level(ENCODER_PHASE_B_0));
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}