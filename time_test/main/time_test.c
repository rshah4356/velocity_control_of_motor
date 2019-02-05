#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void calc_time(){
    TickType_t xtime1,xtime2,xtotal;
    uint32_t time1;
    while(1)
    {
        xtime1 = xTaskGetTickCount();
        time1 = xtime1/portTICK_RATE_MS;
        printf("xtime1 = %u\t",xtime1);
        printf("time1 = %u\t",time1);
        // vTaskDelay(10);
        for(int i = 0;i < 10000;i++)
        {

        }
        xtime2 = xTaskGetTickCount();
        printf("xtime2 = %u\n",xtime2);
        xtotal = xtime2 - xtime1;
        // printf("total = %u\n",xtotal);
        
    }
}

void app_main()
{
    TickType_t xtime3;
    xTaskCreate(calc_time,"clac_time",4096,NULL,10,NULL);
    while(1)
    {
        // xtime3 = xTaskGetTickCountFromISR(); 
        // printf("xtime3 = %u\n",xtime3);
        vTaskDelay(100);    
    }
}
