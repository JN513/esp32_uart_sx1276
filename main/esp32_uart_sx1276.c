#include <stdio.h>
#include <string.h>

#include "esp_system.h" // Biblioteca de funções do sistema
#include "esp_event.h"  // Biblioteca de eventos
#include "esp_log.h"    // Biblioteca de log

#include "freertos/FreeRTOS.h" // Biblioteca de sistema de tarefas
#include "freertos/task.h"     // Biblioteca de tarefas
#include "freertos/semphr.h"   // Biblioteca de sistema de mutex
#include "freertos/queue.h"    // Biblioteca de filas

#include "soc/soc.h" // Biblioteca de configuração do chip

#include "esp_task_wdt.h" // Biblioteca de tarefas do WDT

#include "driver/gpio.h"

#include "driver/uart.h"

#include "uart_lora_library.h"

void app_main(void)
{
    configUart(17, 18);

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}
