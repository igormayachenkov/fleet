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
#include "radio_data.h"

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

#define EXAMPLE_ESP_MAXIMUM_RETRY 3
#define EXAMPLE_ESP_WIFI_SSID      "ITWF"
#define EXAMPLE_ESP_WIFI_PASS      "8jC5f6kM8yPm"

// Wi-Fi authmode type
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD    WIFI_AUTH_WPA2_PSK

// Configuration for SAE PWE derivation.
// Values:
// enumerator WPA3_SAE_PWE_UNSPECIFIED
// enumerator WPA3_SAE_PWE_HUNT_AND_PECK
// enumerator WPA3_SAE_PWE_HASH_TO_ELEMENT
// enumerator WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SAE_MODE   WPA3_SAE_PWE_BOTH

#define EXAMPLE_H2E_IDENTIFIER ""



static void event_handler(
    void* arg, 
    esp_event_base_t event_base,
    int32_t event_id, 
    void* event_data
){
    ESP_LOGI(TAG, " ---> event_base: %s   event_id: %ld", event_base, event_id);

    // if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    //     esp_wifi_connect();
    // } else 
    // if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    //     if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
    //         esp_wifi_connect();
    //         s_retry_num++;
    //         ESP_LOGI(TAG, "retry to connect to the AP");
    //     } else {
    //         xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    //     }
    //     ESP_LOGI(TAG,"connect to the AP fail");
    // } else 
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGW(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);

        send_receive_data();

        ESP_LOGW(TAG, "disconnect");
        esp_wifi_disconnect();

    }
}

void init(void){
    ESP_LOGI(TAG, "init");

    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (password len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e        = ESP_WIFI_SAE_MODE,
            .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );                                                        

    // Start WiFi according to current configuration If mode is WIFI_MODE_STA, it creates station control block and starts station 
    ESP_ERROR_CHECK( esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

//-----------------------------------------------------------------------------------------------------
// THE TASK
void taskRadio( void *pvParameters )
{
    ESP_LOGW(TAG, "***** TASK Radio started, core ID: %i *****" ,xPortGetCoreID());

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    // Init Wi-Fi
    // Don't forget to initialize NVS using nvs_flash_init() before enabling WiFi.
    init();

    while(1){

        ESP_LOGW(TAG, "connect");
        esp_wifi_connect();
        
        vTaskDelay( 30000 / portTICK_PERIOD_MS );
    }
     vTaskDelete( NULL );

}
