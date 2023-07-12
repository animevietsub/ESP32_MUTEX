#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <rom/ets_sys.h>
#include <string.h>

static const char *TAG1 = "[MUTEX1]";
static const char *TAG2 = "[MUTEX2]";
static int8_t result[100];
static int32_t c_index = 0;
static int32_t globalVar = 0;
static portMUX_TYPE mutex1 = portMUX_INITIALIZER_UNLOCKED;
static portMUX_TYPE mutex2 = portMUX_INITIALIZER_UNLOCKED;

void workApp1(void *pvParameters)
{
    while (c_index < 100)
    {
        portENTER_CRITICAL(&mutex1);
        globalVar++;
        result[c_index++] = globalVar;
        portEXIT_CRITICAL(&mutex1);
        vTaskDelay(1);
    }
    ESP_LOG_BUFFER_HEX(TAG1, result, 100);
    vTaskDelete(NULL);
}

void workApp2(void *pvParameters)
{
    while (c_index < 100)
    {
        portENTER_CRITICAL(&mutex1);
        globalVar++;
        ets_delay_us(10000);
        result[c_index++] = globalVar;
        portEXIT_CRITICAL(&mutex1);
        vTaskDelay(1);
    }
    ESP_LOG_BUFFER_HEX(TAG2, result, 100);
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreatePinnedToCore(workApp1, "workApp1", 1024 * 2, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(workApp2, "workApp2", 1024 * 2, NULL, 0, NULL, 1);
    vTaskDelete(NULL);
}
