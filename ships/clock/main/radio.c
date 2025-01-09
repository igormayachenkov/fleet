#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/idf_additions.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "radio.h"
#include "esp_task_wdt.h"


#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"


static const char *TAG = "radio";


/* FreeRTOS event group to signal when we are connected */
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static int s_retry_num = 0;

void init(void){
    ESP_LOGI(TAG, "init");

    s_wifi_event_group = xEventGroupCreate();

}

void connect(void){
    ESP_LOGI(TAG, "connect");

}

//-----------------------------------------------------------------------------------------------------
// THE TASK
void taskRadio( void *pvParameters )
{
    ESP_LOGW(TAG, "***** TASK Radio started, core ID: %i *****" ,xPortGetCoreID());

    init();

    while(1){

        connect();
        
        vTaskDelay( 10000 / portTICK_PERIOD_MS );
    }
     vTaskDelete( NULL );

}
