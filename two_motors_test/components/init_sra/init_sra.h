#ifndef _INIT_SRA_H
#define _INIT_SRA_H
#include "driver/gpio.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"


typedef struct{
    mcpwm_unit_t pwm_unit;
    mcpwm_io_signals_t pwm_operator;
    mcpwm_timer_t pwm_timer;
    gpio_num_t pwm_pin;
}mcpwm_t;

void init_interrupt(gpio_num_t NUM_GPIO);

void init_gpio(gpio_num_t NUM_GPIO, gpio_mode_t GPIO_MODE);

void init_mcpwm(mcpwm_t *motor);


#endif