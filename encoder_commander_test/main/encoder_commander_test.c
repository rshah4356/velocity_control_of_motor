#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../../components/init_sra/init_sra.h"
#include "../../components/pin_defs/pin_defs.h"
#include "../../components/encoder_commander/encoder_commander.h"

encoder_commander_t encoder_0 = {.name = "ENCODER_0", .id = 0, .enc_intr = ENCODER_0_A, .enc_dir = ENCODER_0_B, .ticks_count = 0};

void app_main()
{
    init_encoder(&encoder_0);
    xTaskCreate((TaskFunction_t)print_encoder_status, "print_encoder_status", 8192, (void*) &encoder_0, 11, NULL);

    while(1) 
    {
        vTaskDelay(1 / portTICK_RATE_MS);
    }
}