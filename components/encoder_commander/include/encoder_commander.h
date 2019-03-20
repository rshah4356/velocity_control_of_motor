#ifndef _ENCODER_H
#define _ENCODER_H

#include "init_sra.h"

typedef struct{
    char  name[10];
    int   id;
    int   curr_rpm;
    volatile long ticks_count;
    volatile long total_ticks;
    gpio_num_t enc_intr;
    gpio_num_t enc_dir;
    esp_timer_handle_t periodic_timer;
}encoder_commander_t;

void init_encoder(encoder_commander_t *encoder);    //TODO: add parameter for type of edge

void IRAM_ATTR enc_isr_handler(encoder_commander_t* encoder);

void IRAM_ATTR calculate_rpm(encoder_commander_t* encoder);

void setup_rpm_calculator(encoder_commander_t* encoder);

void IRAM_ATTR uart_pub(encoder_commander_t* encoder);

void setup_uart_pub(encoder_commander_t* encoder);

void print_encoder_status(encoder_commander_t* encoder);

#endif