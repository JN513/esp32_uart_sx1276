#include <stdio.h>
#include <string.h>
#include "esp_log.h" // Biblioteca de log

#include "freertos/FreeRTOS.h" // Biblioteca de sistema de tarefas
#include "freertos/task.h"     // Biblioteca de tarefas
#include "freertos/semphr.h"   // Biblioteca de sistema de mutex
#include "freertos/queue.h"    // Biblioteca de filas

#include "uart_lora_library.h"

static const char *TAG = "esp";
void callback(uint8_t *data, size_t len)
{
    ESP_LOGI(TAG, "[UART STR]: %s", data);
}

void app_main(void)
{
    configUart(17, 18);
    setCallback(*callback);

    while (1)
    {
        sendData("Ola");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
