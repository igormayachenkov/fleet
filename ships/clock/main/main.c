#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"


static const char *TAG = "clock";
static uint8_t ledState = 0;
#define GPIO_OUTPUT  19


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
    io_conf.pin_bit_mask = 1ULL<<GPIO_OUTPUT ; //((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    int error;
    while(1){

        // LED
        ledState = !ledState;
        ESP_LOGI(TAG, "ledState: %s!", ledState == true ? "ON" : "OFF");
        gpio_set_level(GPIO_OUTPUT, ledState);

        
        vTaskDelay( 3000 / portTICK_PERIOD_MS );
    }

}

