#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"


static const char *TAG = "clock";

// CHANNELS LIST
static gpio_num_t channels[4] = {13,12,14,27};
static uint8_t    channel = 0; // current channel

#define CHANNEL_ON  0
#define CHANNEL_OFF 1

void app_main(void)
{
    int pt = portTICK_PERIOD_MS;
    ESP_LOGI(TAG, "***** APP STARTED *****" );
    ESP_LOGI(TAG, "portTICK_PERIOD_MS: %u", pt);

    // CONFIG OUTPUT
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = ((1ULL<<channels[0]) | (1ULL<<channels[1]) | (1ULL<<channels[2]) | (1ULL<<channels[3]));
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    while(1){

        // Hide old channel
        gpio_set_level(channels[channel], CHANNEL_OFF);

        // Select new channel
        channel++;
        if(channel==4) channel=0;

        // Show new channel
        gpio_set_level(channels[channel], CHANNEL_ON);



        ESP_LOGI(TAG, "channel: %d", channel);

        
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }

}

