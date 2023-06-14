
#include "uart_lora_library.h"

void (*callback_function)(uint8_t *data, size_t len) = NULL;

void configUart(int txpin, int rxpin)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(UART_NUM_1, BUFF_SIZE * 2, BUFF_SIZE * 2, 20, &uart_queue, 0);

    uart_param_config(UART_NUM_1, &uart_config);

    uart_set_pin(UART_NUM_1, txpin, rxpin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}

int sendData(const char *data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(LORA_TAG, "Enviou %d bytes", txBytes);

    return txBytes;
}

void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buff_size;
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * BUFF_SIZE);

    while (1)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            bzero(data, BUFF_SIZE); // tranforma em zero todos os elementos do meu vetor de caracteres

            ESP_LOGI(LORA_TAG, "uart[%d] event:", 1);

            switch (event.type)
            {
            // Event of UART receving data
            case UART_DATA: // Recebe algo
                ESP_LOGI(LORA_TAG, "[UART DATA]: %d", event.size);
                uart_read_bytes(UART_NUM_1, data, event.size, portMAX_DELAY);
                callback_function(data, event.size);
                break;
            // Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGI(LORA_TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(UART_NUM_1);
                xQueueReset(uart_queue);
                break;
            // Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGI(LORA_TAG, "ring buffer full");
                uart_flush_input(UART_NUM_1);
                xQueueReset(uart_queue);
                break;
            // Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGI(LORA_TAG, "uart rx break");
                break;
            // Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGI(LORA_TAG, "uart parity error");
                break;
            // Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGI(LORA_TAG, "uart frame error");
                break;
            // UART_PATTERN_DET
            case UART_PATTERN_DET:
                uart_get_buffered_data_len(UART_NUM_1, &buff_size);
                int pos = uart_pattern_pop_pos(UART_NUM_1);
                ESP_LOGI(LORA_TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buff_size);
                if (pos == -1)
                {
                    // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                    // record the position. We should set a larger queue size.
                    // As an example, we directly flush the rx buffer here.
                    uart_flush_input(UART_NUM_1);
                }
                else
                {
                    uart_read_bytes(UART_NUM_1, data, pos, 100 / portTICK_PERIOD_MS);
                    uint8_t pat[PATTERN_CHR_NUM + 1];
                    memset(pat, 0, sizeof(pat));
                    uart_read_bytes(UART_NUM_1, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                    ESP_LOGI(LORA_TAG, "read data: %s", data);
                    ESP_LOGI(LORA_TAG, "read pat : %s", pat);
                }
                break;
            // Others
            default:
                ESP_LOGI(LORA_TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }

    free(data);
    data = NULL;
    vTaskDelete(NULL);
}

void setCallback(void (*callback)(uint8_t *data, size_t len))
{
    callback_function = callback;
}