/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <string.h>

/**
 * This is an example which echos any data it receives on configured UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: on
 * - Flow control: off
 * - Event queue: 0
 * - Pin assignment: see defines below (See Kconfig)
 */

#define ECHO_TEST_TXD (17)
#define ECHO_TEST_RXD (16)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM (2)
#define ECHO_UART_BAUD_RATE (9600)
#define ECHO_TASK_STACK_SIZE (CONFIG_EXAMPLE_TASK_STACK_SIZE)
#define BUF_SIZE (1024)

#define button (2)

static const char *TAG = "UART TEST";

static void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(ECHO_UART_PORT_NUM, &uart_config);

    uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);

    uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);

    ESP_LOGI(TAG, "init uart completed!");

    // Configure a temporary buffer for the incoming data
    //uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    int data[3]={22,10,1995};
    while (1)
    {

        // Send Buffer char.
        char *test_str = "This is a test string";
        uart_write_bytes(ECHO_UART_PORT_NUM, (const char *)test_str, strlen(test_str));

        ESP_LOGI(TAG, "Recv str: %s bytes %d", (char *)test_str, strlen(test_str));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
     


        
    }
}

esp_err_t init_led(void)
{
    gpio_reset_pin(button);
    gpio_set_direction(button, GPIO_MODE_INPUT);
    return ESP_OK;
}
void app_main(void)
{

    init_led();
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
}
