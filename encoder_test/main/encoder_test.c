#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../components/base_nav/base_nav.h"
#include "../components/pin_defs_for_single_motor/pin_defs_for_single_motor.h"

static volatile int16_t ticks_count_0 = 0;

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
        // printf("The Encoder ticks for motor 0: %d\n", ticks_count_0);
        printf("The Encoder ticks for motor 0: %d\n", ticks_count_0);
    }
}

void app_main()
{
    init_interrupt(ENCODER_PHASE_A_0);
    init_gpio(ENCODER_PHASE_B_0,GPIO_MODE_INPUT);
    
    gpio_isr_handler_add(ENCODER_PHASE_A_0, gpio_isr_handler_0, (void*) ENCODER_PHASE_B_0);
    
    xTaskCreate(print_ticks, "print_ticks", 2048, NULL, 10, NULL);
        
    while(1) 
    {
        // printf("gpio = %d\t%d\n", gpio_get_level(ENCODER_PHASE_A_0), gpio_get_level(ENCODER_PHASE_B_0));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}