#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

// #define calc_veloctiy_timer 55555   //111111
// #define calc_veloctiy_timer_multiplier (1000000*60)/(135*calc_veloctiy_timer)

#include "../components/init_sra/init_sra.h"
#include "../components/motor_commander/motor_commander.h"

motor_commander_t motor_0 = (motor_commander_t) {.name = "LEFT_MOTOR", .id = 0, .curr_velocity = 0, .desr_velocity = 0, .Kp = 0.005, .enc_intr = ENCODER_PHASE_0_A, .enc_dir = ENCODER_PHASE_0_B, .pwm_0 = {}.pwm_unit = MCPWM_UNIT_0, .pwm.pwm_operator = MCPWM0A, .pwm.pwm_timer = MCPWM_TIMER_0, .pwm.pwm_pin = MOTOR_0_PWM };

void app_main()
{
    init_motor(motor_0);
    xTaskCreate(drive_motor, "drive_motor", 8192, &motor_0, 23, NULL);

    while(1) 
    {
        // printf("gpio = %d\t%d\n", gpio_get_level(ENCODER_PHASE_A_0), gpio_get_level(ENCODER_PHASE_B_0));
        vTaskDelay(1 / portTICK_RATE_MS);
    }
}