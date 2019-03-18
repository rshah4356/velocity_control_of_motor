#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "init_sra.h"
#include "pin_defs.h"
#include "encoder_commander.h"

encoder_commander_t encoder_0 = {.name = "ENCODER_0", .id = 0, .enc_intr = ENCODER_R_A, .enc_dir = ENCODER_R_B, .ticks_count = 0};

encoder_commander_t encoder_1 = {.name = "ENCODER_1", .id = 1, .enc_intr = ENCODER_L_A, .enc_dir = ENCODER_L_B, .ticks_count = 0};

void app_main()
{
    init_encoder(&encoder_0);
    init_encoder(&encoder_1);
    xTaskCreate((TaskFunction_t)print_encoder_status, "print_encoder_status_0", 8192, (void*) &encoder_0, 11, NULL);
    xTaskCreate((TaskFunction_t)print_encoder_status, "print_encoder_status_1", 8192, (void*) &encoder_1, 11, NULL);
    while(1) 
    {
        vTaskDelay(1 / portTICK_RATE_MS);
    }
}