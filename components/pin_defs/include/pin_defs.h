#ifndef PIN_DEFS_H
    #define PIN_DEFS

    #define MOTOR_F_PWM_A   13
    #define MOTOR_F_PWM_B   14
    #define ENCODER_F_A     33
    #define ENCODER_F_B     32
    
    #define MOTOR_R_PWM_A   19
    #define MOTOR_R_PWM_B   18
    #define ENCODER_R_A     34
    #define ENCODER_R_B     35
    
    #define MOTOR_B_PWM_A   21
    #define MOTOR_B_PWM_B   22
    #define ENCODER_B_A     27
    #define ENCODER_B_B     26
    
    #define MOTOR_L_PWM_A   25
    #define MOTOR_L_PWM_B   23
    #define ENCODER_L_A     36
    #define ENCODER_L_B     39
    
    #define ECHO_TEST_TXD  (1)
    #define ECHO_TEST_RXD  (3)
    #define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
    #define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

    #define BUF_SIZE (1024)

#endif