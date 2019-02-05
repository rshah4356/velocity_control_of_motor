#include "init_sra.h"
#define ESP_INTR_FLAG_DEFAULT 0

static gpio_config_t io_conf;

void init_interrupt(gpio_num_t NUM_GPIO){
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = ((uint64_t)1 << NUM_GPIO);
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
}

void init_gpio(gpio_num_t NUM_GPIO, gpio_mode_t GPIO_MODE){
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE;
    io_conf.pin_bit_mask = ((uint64_t)1 << NUM_GPIO);
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);

}
void init_mcpwm(mcpwm_t *motor){
    mcpwm_gpio_init(motor->pwm_unit, motor->pwm_operator, motor->pwm_pin);
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(motor->pwm_unit, motor->pwm_timer, &pwm_config);
}