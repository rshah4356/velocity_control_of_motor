#include "freertos/FreeRTOS.h"
#include "esp_timer.h"
#include "freertos/task.h"
#include "encoder_commander.h"
// #include "../init_sra/init_sra.h"
#include "pin_defs.h"
#include "driver/uart.h"



void IRAM_ATTR enc_isr_handler(encoder_commander_t* encoder)
{
    if(gpio_get_level(encoder->enc_dir)== 0){
        encoder->ticks_count++;
    }
    else{
        encoder->ticks_count--;
    }
}

void init_encoder(encoder_commander_t* encoder){
    printf("init_encoder()\n");
    init_gpio(encoder->enc_dir, GPIO_MODE_INPUT);
    init_interrupt(encoder->enc_intr);
	gpio_isr_handler_add(encoder->enc_intr, enc_isr_handler, (void*) encoder);
    setup_rpm_calculator(encoder);
    // setup_uart_pub(encoder);
    printf("init_encoder() END\n");
}



void calculate_rpm(encoder_commander_t* encoder){
    encoder->curr_rpm = encoder->ticks_count * 10.0001;    //(1000000*60)/(135*111111) = 4.0000400004
    encoder->total_ticks +=  encoder->ticks_count;
    encoder->ticks_count = 0;
    vTaskDelay(1 / portTICK_RATE_MS);
}

void setup_rpm_calculator(encoder_commander_t* encoder){
    printf("setup_rpm_calculator()\n");
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = calculate_rpm,
            .arg = encoder,
            .name = encoder->name
    };

    // esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &(encoder->periodic_timer));
    esp_timer_start_periodic(encoder->periodic_timer, 44444);
    printf("setup_rpm_calculator() ENDS\n");
}

void print_encoder_status(encoder_commander_t *encoder){
    while(true){
        printf("%s:\tticks: %ld\trpm: %d\n", encoder->name, encoder->ticks_count, encoder->curr_rpm);
    }
}

void uart_pub(encoder_commander_t* encoder){
    // static int *data = 0;
    // *data = *data + 1 ;
    // *data = encoder->total_ticks + 41 ;
    // if(encoder->id == 3){
    //     *data |= 128;
    // }
    // uart_write_bytes(UART_NUM_2, (const char *)data, 1);
    // uart_write_bytes(UART_NUM_2, 'a', 1);
    // encoder->total_ticks = 0;
    vTaskDelay(10 / portTICK_RATE_MS);
}

void setup_uart_pub(encoder_commander_t* encoder){
    printf("setup_uart_pub()\n");
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = uart_pub,
            .arg = encoder,
            .name = encoder->name
    };
    // esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &(encoder->periodic_timer));
    esp_timer_start_periodic(encoder->periodic_timer, 1333332);
    printf("setup_uart_pub() ENDS\n");
}
