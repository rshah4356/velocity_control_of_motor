#include "base_nav.h"

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
void init_pwm(motor_pwm_t *motor){
    mcpwm_gpio_init(motor->pwm_unit, motor->pwm_operator, motor->pwm_pin);
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(motor->pwm_unit, motor->pwm_timer, &pwm_config);
}

void init_motor(motor_commander_t *motor){
    init_gpio(motor->dir_0_pin, GPIO_MODE_OUTPUT);
    init_gpio(motor->dir_1_pin, GPIO_MODE_OUTPUT);
    init_pwm(&(motor->pwm_pin));
}

void drive_motor(motor_commander_t *motor){
    init_motor(motor);
    static int err ;
    static int prev_err ;
    static int cum_err;
    int pwm = 0;
    
    err = motor->desr_velocity - motor->curr_velocity;
    prev_err = motor->curr_velocity - motor->prev_velocity;
    cum_err += err;
    pwm = KP*err - KD*(err-prev_err) + KI*(cum_err);
    motor->prev_velocity = motor->curr_velocity;
    
    if(pwm > 0)
    {
        gpio_set_level(motor->dir_0_pin,0);
        gpio_set_level(motor->dir_1_pin,1);
    }
    if(pwm < 0)
    {
        gpio_set_level(motor->dir_0_pin,1);
        gpio_set_level(motor->dir_1_pin,0);
    }
    mcpwm_set_duty(motor->pwm_pin.pwm_unit, motor->pwm_pin.pwm_timer, motor->pwm_pin.pwm_operator, abs(pwm));
    mcpwm_set_duty_type(motor->pwm_pin.pwm_unit, motor->pwm_pin.pwm_timer, motor->pwm_pin.pwm_operator, MCPWM_DUTY_MODE_0);

}