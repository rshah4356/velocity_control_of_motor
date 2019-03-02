#ifndef PIN_DEFS_H
    #define PIN_DEFS

    #define MOTOR_F_PWM_A   12
    #define MOTOR_F_PWM_B   13
    #define ENCODER_F_A     33
    #define ENCODER_F_B     32
    
    #define MOTOR_R_PWM_A   18
    #define MOTOR_R_PWM_B   19
    #define ENCODER_R_A     35
    #define ENCODER_R_B     34
    
    #define MOTOR_B_PWM_A   22
    #define MOTOR_B_PWM_B   21
    #define ENCODER_B_A     27
    #define ENCODER_B_B     26
    
    #define MOTOR_L_PWM_A   25
    #define MOTOR_L_PWM_B   23
    #define ENCODER_L_A     39
    #define ENCODER_L_B     36
    
    #define ECHO_TEST_TXD  (4)
    #define ECHO_TEST_RXD  (5)
    #define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
    #define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

    #define BUF_SIZE (1024)

#endif