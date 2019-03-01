#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "motor_commander.h"
#include "pin_defs.h"

void init_motor(motor_commander_t* motor){
    init_mcpwm(&(motor->pwm_A));
    init_mcpwm(&(motor->pwm_B));
    init_encoder(&(motor->encoder));
}

void calculate_duty_cycle(motor_commander_t* motor){
    motor->err = motor->desr_rpm - motor->encoder.curr_rpm;
    motor->prev_err = motor->err;
    motor->cum_err += motor->err;
    motor->del_duty_cycle = (motor->Kp)*motor->err - (motor->Kd)*(motor->err-motor->prev_err) + (motor->Ki)*(motor->cum_err);
    motor->duty_cycle += motor->del_duty_cycle;
    if ((motor->err ^ motor->prev_err) & (1 << 15)){  //check if err * prev_error < 0
        motor->cum_err = 0;
    }
    if(motor->desr_rpm > 0){
        if(motor->duty_cycle < 1)
            motor->duty_cycle = 1;
    }
    else if(motor->desr_rpm < 0){
        if(motor->duty_cycle > -1)
            motor->duty_cycle = -1;
    }
            
    if(motor->duty_cycle > 100)
        motor->duty_cycle = 100;
    else if(motor->duty_cycle < -100)
        motor->duty_cycle = -100;
}

void write_duty_cycle(motor_commander_t* motor){
        // printf("MOTOR: %s,\tDES: %d,\tCURR: %d,\t", motor->name, motor->desr_rpm, motor->encoder.curr_rpm);
        // printf("pwm_pins: %d, %d,\t", motor->pwm_A.pwm_pin, motor->pwm_B.pwm_pin);
        // printf("err: %d,\tdcyc: %f,\tdel_dcyc: %f\tKP: %f\n", motor->err, motor->duty_cycle, motor->del_duty_cycle, motor->Kp);
    if(motor->duty_cycle > 0){
        if(motor->duty_cycle > 100)
            motor->duty_cycle = 100;
        mcpwm_set_duty(motor->pwm_A.pwm_unit, motor->pwm_A.pwm_timer, motor->pwm_A.pwm_operator, 0);
        mcpwm_set_duty_type(motor->pwm_A.pwm_unit, motor->pwm_A.pwm_timer, motor->pwm_A.pwm_operator, MCPWM_DUTY_MODE_0);
        // gpio_set_level(motor->pwm_A.pwm_pin, 0);
        mcpwm_set_duty(motor->pwm_B.pwm_unit, motor->pwm_B.pwm_timer, motor->pwm_B.pwm_operator, abs(motor->duty_cycle));
        mcpwm_set_duty_type(motor->pwm_B.pwm_unit, motor->pwm_B.pwm_timer, motor->pwm_B.pwm_operator, MCPWM_DUTY_MODE_0);
    }
    else if(motor->duty_cycle < 0){
        if(motor->duty_cycle < -100)
            motor->duty_cycle = -100;
        mcpwm_set_duty(motor->pwm_A.pwm_unit, motor->pwm_A.pwm_timer, motor->pwm_A.pwm_operator, abs(motor->duty_cycle));
        mcpwm_set_duty_type(motor->pwm_A.pwm_unit, motor->pwm_A.pwm_timer, motor->pwm_A.pwm_operator, MCPWM_DUTY_MODE_0);
        // gpio_set_level(motor->pwm_B.pwm_pin, 0);
        mcpwm_set_duty(motor->pwm_B.pwm_unit, motor->pwm_B.pwm_timer, motor->pwm_B.pwm_operator, 0);
        mcpwm_set_duty_type(motor->pwm_B.pwm_unit, motor->pwm_B.pwm_timer, motor->pwm_B.pwm_operator, MCPWM_DUTY_MODE_0);
    }
    else{
        mcpwm_set_duty(motor->pwm_A.pwm_unit, motor->pwm_A.pwm_timer, motor->pwm_A.pwm_operator, 100);
        mcpwm_set_duty_type(motor->pwm_A.pwm_unit, motor->pwm_A.pwm_timer, motor->pwm_A.pwm_operator, MCPWM_DUTY_MODE_0);
        // gpio_set_level(motor->pwm_B.pwm_pin, 0); 
        mcpwm_set_duty(motor->pwm_B.pwm_unit, motor->pwm_B.pwm_timer, motor->pwm_B.pwm_operator, 100);
        mcpwm_set_duty_type(motor->pwm_B.pwm_unit, motor->pwm_B.pwm_timer, motor->pwm_B.pwm_operator, MCPWM_DUTY_MODE_0);
    }
    // else{
    //     gpio_set_level(motor->pwm_A.pwm_pin, 0);
    //     gpio_set_level(motor->pwm_B.pwm_pin, 0);
    // }
}

void drive_motor(motor_commander_t* motor){
    // init_motor(motor);
    while(1){
        // calculate_duty_cycle(motor);
        write_duty_cycle(motor);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void print_motor_status(motor_commander_t* motor){
    while(true){
        printf("%s:,\tDES: %d,\tCURR: %d,\t", motor->name, motor->desr_rpm, motor->encoder.curr_rpm);
        printf("dc: %f,\tdel_dc: %f\tKP: %f\tKD: %f\tmode: %d\terr: %d\t\n", motor->duty_cycle, motor->del_duty_cycle, motor->Kp, motor->Kd, motor->tune_mode, motor->err);
        vTaskDelay(25 / portTICK_RATE_MS);
    }
}
/*
void tune_pid(motor_commander_t* motor){
    motor->tune_mode = false;   //Kp - false; Kd - true
    while(true){
        vTaskDelay(40 / portTICK_RATE_MS);
        printf("%d\t%d\t%d\n", gpio_get_level(SWITCH_MODE), gpio_get_level(SWITCH_INC), gpio_get_level(SWITCH_DEC));
        if(!gpio_get_level(SWITCH_MODE)){
            while(!gpio_get_level(SWITCH_MODE));
            motor->tune_mode = !motor->tune_mode;
        }
        else if(!gpio_get_level(SWITCH_INC)){
            while(!gpio_get_level(SWITCH_INC));
            if(motor->tune_mode)
                motor->Kd += KD_STEP;
            else
                motor->Kp += KP_STEP;
        }
        else if(!gpio_get_level(SWITCH_INC)){
            while(!gpio_get_level(SWITCH_INC));
            if(motor->tune_mode)
                motor->Kd -= KD_STEP;
            else
                motor->Kp -= KP_STEP;
        }
    }
}
*/