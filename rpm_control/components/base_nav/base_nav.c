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
    init_pwm(&(motor->pwm));
}

void print_motor_status(motor_commander_t *motor){
    while(1){
        printf("\nMOTOR: %s,\tDES: %d,\tCURR: %d,\tDCYC:%f", motor->name, motor->desr_velocity, motor->curr_velocity, motor->duty_cycle);
    }
}

void drive_motor(motor_commander_t *motor){
    init_motor(motor);
    static int err;
    static int prev_err;
    static int cum_err;
    motor->duty_cycle = 0;
    
    while(1){
        prev_err = err;
        err = motor->desr_velocity - motor->curr_velocity;
        cum_err += err;
        motor->delta_duty_cycle = KP*err + KD*(err-prev_err) + KI*(cum_err);
        motor->duty_cycle += motor->delta_duty_cycle;
        if ((err ^ prev_err) & (1 << 15)){  //check if err * prev_error < 0
            // printf("\n\n\n\n\nerr: %d, prev_err: %d, cum_err: %d\n\n\n\n\n\n\n", err, prev_err, cum_err);
            cum_err = 0;
        }
        if(motor->desr_velocity > 0){
            if(motor->duty_cycle < 1)
                motor->duty_cycle = 1;
        }
        else if(motor->desr_velocity < 0){
            if(motor->duty_cycle > -1)
                motor->duty_cycle = -1;
        }
                
        if(motor->duty_cycle > 100)
            motor->duty_cycle = 100;
        else if(motor->duty_cycle < -100)
            motor->duty_cycle = -100;

        if(motor->duty_cycle > 0){
            gpio_set_level(motor->dir_0_pin,1);
            gpio_set_level(motor->dir_1_pin,0);
        }
        else if(motor->duty_cycle < 0){
            gpio_set_level(motor->dir_0_pin,0);
            gpio_set_level(motor->dir_1_pin,1);
        }
        else{
            gpio_set_level(motor->dir_0_pin,1);
            gpio_set_level(motor->dir_1_pin,1);
            // motor->duty_cycle = 100;
        }
        printf("\nMOTOR: %s,\tDES: %d,\tCURR: %d,\tDCYC:%f\t", motor->name, motor->desr_velocity, motor->curr_velocity, motor->duty_cycle);
        printf("err: %d,\tdcyc: %f,\tdel_dcyc: %f\tKP: %f", err, motor->duty_cycle, motor->delta_duty_cycle, KP);
        mcpwm_set_duty(motor->pwm.pwm_unit, motor->pwm.pwm_timer, motor->pwm.pwm_operator, abs(motor->duty_cycle));
        mcpwm_set_duty_type(motor->pwm.pwm_unit, motor->pwm.pwm_timer, motor->pwm.pwm_operator, MCPWM_DUTY_MODE_0);
    }
}