#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "radio_data.h"

static const char *TAG = "radio_data";

void send_receive_data(){
    ESP_LOGW(TAG, "send_receive_data");
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
}