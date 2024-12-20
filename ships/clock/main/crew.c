#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/idf_additions.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "crew.h"
#include "esp_task_wdt.h"

/**
 * TASK WATCHDOG
 * To disable it
 * in "sdkconfig" file
 *     CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1=n
 **/

static const char *TAG = "crew";


// CHANNELS LIST
#define CHANNELS_N 4
static gpio_num_t channels[CHANNELS_N] = {16,17,18,19};// works
static uint8_t    channel = 0; // current channel
#define CHANNEL_ON  0
#define CHANNEL_OFF 1

// SEGMENTS LIST
#define SEGMENTS_N 7
static gpio_num_t segments[SEGMENTS_N] = {13,12,14,27,26,25,33};

// SYMBOLS in 7_seg indicator
//   a
// f   b
//   g
// e   c
//   d    [dp]
#define SYMBOLS_N 10
static uint8_t symbols[SYMBOLS_N][SEGMENTS_N] = {
    //   a,b,c,d,e,f,g
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };

// VIDEO MEMORY (symbols for channels)
static uint8_t videoMemory[CHANNELS_N] = {0,1,2,3};
#define MIN_L  0 
#define MIN_H  1
#define HOUR_L 2
#define HOUR_H 3

void updateTime(int h, int m){
    int digit; 
    ESP_LOGI(TAG, "updateTime(h,m): %i %i",h,m);

    // Update video memory
    digit = m/10; // min high
    videoMemory[MIN_H] = digit;
    digit = m-digit*10; // min low
    videoMemory[MIN_L] = digit;
    
    digit = h/10; // hour high
    videoMemory[HOUR_H] = digit;
    digit = h-digit*10; // hour low
    videoMemory[HOUR_L] = digit;
    
    ESP_LOGI(TAG, "    video memory :  %i %i %i %i", videoMemory[3], videoMemory[2], videoMemory[1], videoMemory[0]);
}


//-----------------------------------------------------------------------------------------------------
// THE TASK
void taskCrew( void *pvParameters )
{
    uint8_t i;
    ESP_LOGW(TAG, "***** TASK Crew started, core ID: %i *****" ,xPortGetCoreID());

    // CONFIG OUTPUT
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 0;
    for( i=0; i<CHANNELS_N; i++ ) io_conf.pin_bit_mask |= (1ULL<<channels[i]);
    for( i=0; i<SEGMENTS_N; i++ ) io_conf.pin_bit_mask |= (1ULL<<segments[i]);
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    // Hide channels before
    for(i=0; i<CHANNELS_N; i++)
        gpio_set_level(channels[i], CHANNEL_OFF);


    while(1){

        // Hide old channel
        gpio_set_level(channels[channel], CHANNEL_OFF);

        ets_delay_us(500); 


        // Select new channel
        channel++;
        if(channel==CHANNELS_N) channel=0;

        // Update pins according to "video memory" (segments for the channel)
        uint8_t* symbol = symbols[videoMemory[channel]]; // Get current symbol for the channel
        for(i=0; i<SEGMENTS_N; i++)
            gpio_set_level(segments[i], symbol[i]); // Set segment value

        // Show new channel
        gpio_set_level(channels[channel], CHANNEL_ON);



        //ESP_LOGI(TAG, "channel: %d", channel);

        
        ets_delay_us(500); 
        //vTaskDelay( 100 / portTICK_PERIOD_MS );

        //esp_task_wdt_reset(); do not!
    }

    /* Tasks must not attempt to return from their implementing
       function or otherwise exit. In newer FreeRTOS port
       attempting to do so will result in an configASSERT() being
       called if it is defined. If it is necessary for a task to
       exit then have the task call vTaskDelete( NULL ) to ensure
       its exit is clean. */
    vTaskDelete( NULL );
}


