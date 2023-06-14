#ifndef __LORA_UART_LIBRARY_H__
#define __LORA_UART_LIBRARY_H__

#include <stdio.h>
#include <string.h>
#include "esp_log.h" // Biblioteca de log

#include "freertos/FreeRTOS.h" // Biblioteca de sistema de tarefas
#include "freertos/task.h"     // Biblioteca de tarefas
#include "freertos/semphr.h"   // Biblioteca de sistema de mutex
#include "freertos/queue.h"    // Biblioteca de filas

#include "driver/gpio.h"
#include "driver/uart.h"

#include "definitions.h"

// #define BUFF_SIZE 1024
// #define PATTERN_CHR_NUM 3
// #define BROADCAST_ADDRESS 0xFF

static const char *LORA_TAG = "SX1276_UART";

static QueueHandle_t uart_queue;

typedef struct
{
    gpio_num_t rxPin;
    gpio_num_t txPin;
    gpio_num_t m0Pin;
    gpio_num_t m1Pin;
    gpio_num_t auxPin;

    int baudRate;
    int frequency;
    uint8_t mode;

    uart_port_t uart_port;
} e32_config_t;

int sendData(const char *data);
void configUart(int txpin, int rxpin);
void uart_event_task(void *pvParameters);
void setCallback(void (*callback)(uint8_t *data, size_t len));

void setMode(uint8_t mode);
void SetAddress(uint16_t val);
void SetAddressH(uint8_t val);
void SetAddressL(uint8_t val);
void SetAirDataRate(uint8_t val);
void SetUARTBaudRate(uint8_t val);
void SetSpeed(uint8_t val);
void SetOptions(uint8_t val);
void SetChannel(uint8_t val);
void SetParityBit(uint8_t val);

// functions to set the options
void SetTransmissionMode(uint8_t val);
void SetPullupMode(uint8_t val);
void SetWORTIming(uint8_t val);
void SetFECMode(uint8_t val);
void SetTransmitPower(uint8_t val);

uint8_t getMode();
uint16_t GetAddress();

// methods to get module data
uint8_t GetModel();
uint8_t GetVersion();
uint8_t GetFeatures();

uint8_t GetAddressH();
uint8_t GetAddressL();
uint8_t GetAirDataRate();
uint8_t GetUARTBaudRate();
uint8_t GetChannel();
uint8_t GetParityBit();
uint8_t GetTransmissionMode();
uint8_t GetPullupMode();
uint8_t GetWORTIming();
uint8_t GetFECMode();
uint8_t GetTransmitPower();

uint8_t GetOptions();
uint8_t GetSpeed();

#endif