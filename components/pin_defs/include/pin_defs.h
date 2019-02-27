#ifndef PIN_DEFS_H
    #define PIN_DEFS

    #define MOTOR_F_PWM_A   GPIO_NUM_13
    #define MOTOR_F_PWM_B   GPIO_NUM_14
    #define ENCODER_F_A     GPIO_NUM_33
    #define ENCODER_F_B     GPIO_NUM_32
    
    #define MOTOR_R_PWM_A   GPIO_NUM_18
    #define MOTOR_R_PWM_B   GPIO_NUM_19
    #define ENCODER_R_A     GPIO_NUM_35
    #define ENCODER_R_B     GPIO_NUM_34
    
    #define MOTOR_B_PWM_A   GPIO_NUM_21
    #define MOTOR_B_PWM_B   GPIO_NUM_22
    #define ENCODER_B_A     GPIO_NUM_27
    #define ENCODER_B_B     GPIO_NUM_26
    
    #define MOTOR_L_PWM_A   GPIO_NUM_23
    #define MOTOR_L_PWM_B   GPIO_NUM_25
    #define ENCODER_L_A     GPIO_NUM_39
    #define ENCODER_L_B     GPIO_NUM_36
    
    #define ECHO_TEST_TXD  (GPIO_NUM_4)
    #define ECHO_TEST_RXD  (GPIO_NUM_5)
    #define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
    #define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

    #define BUF_SIZE (1024)

#endif