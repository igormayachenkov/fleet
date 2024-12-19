#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/idf_additions.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "crew.h"


static const char *TAG = "clock";

void app_main(void)
{
    ESP_LOGW(TAG, "***** APP started, core ID: %i   portTICK_PERIOD_MS: %lu *****", xPortGetCoreID(), portTICK_PERIOD_MS);

    BaseType_t xReturned; // Used to pass out the created task's handle. 
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreatePinnedToCore(
        taskCrew,       /* Function that implements the task. */
        "Crew",          /* Text name for the task. */
        4096,      /* Stack size in words, not bytes. */
        ( void * ) 1,    /* Parameter passed into the task. */
        tskIDLE_PRIORITY+10,/* Priority at which the task is created. */
        &xHandle,
        1 //core ID
    );      
    if( xReturned == pdPASS )  ESP_LOGI(TAG, "Task Crew created, prio: %i",tskIDLE_PRIORITY);
    else                       ESP_LOGE(TAG, "Task Crew NOT created");
    

    while(1){

        ESP_LOGI(TAG, "The main loop...");

        
        vTaskDelay( 8000 / portTICK_PERIOD_MS );
    }
     vTaskDelete( NULL );

}

