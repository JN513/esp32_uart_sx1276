#ifndef __LORA_UART_LIBRARY_H__
#define __LORA_UART_LIBRARY_H__

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

#define BUFF_SIZE 1024
#define PATTERN_CHR_NUM 3

const char *TAG = "SX1276_UART";

static QueueHandle_t uart_queue;

static int sendData(const char *data);
static void configUart(int txpin, int rxpin);
static void uart_event_task(void *pvParameters);
static void callback(uint8_t *data, size_t len);
static void setCallback();

#endif