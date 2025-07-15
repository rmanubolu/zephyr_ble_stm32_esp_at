/*
 * Copyright (c) 2025 Ramesh Manubolu
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <stdlib.h>

// sleep time
#define SLEEP_TIME_MS   10

// On-board LED
#define LED_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

// UART1
#define UART1_NODE DT_NODELABEL(usart1)
static const struct device* uart1_dev = DEVICE_DT_GET(UART1_NODE);

/* uart configuration structure */
const struct uart_config uart_cfg = {
    .baudrate = 115200,
    .parity = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
    .data_bits = UART_CFG_DATA_BITS_8,
    .flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

/* UART RX interrupt callback */
static void uart_cb(const struct device *dev, void *ctx)
{
    ARG_UNUSED(ctx);
    uint8_t c;
    while (uart_irq_update(dev) && uart_irq_is_pending(dev) && (dev == uart1_dev))
    {
        if (uart_irq_rx_ready(dev))
        {
            uart_fifo_read(dev, &c, 1);
            printk("USART1: %c (0x%02X)\n", c, c); // Logs via USART2
        }
    }
}

int main(void)
{
    int ret = 0;

    printk("Starting STM32 + ESP32 BLE peripheral controller\n");

    if (!device_is_ready(led.port) || !device_is_ready(uart1_dev))
    {
        printk("Device init failed\n"); 
        return ret;
    }

    printk("uart1_dev: %p, ready: %d\n", uart1_dev, device_is_ready(uart1_dev));

    // LED configuration
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    /* uart configuration parameters */
	int err = uart_configure(uart1_dev, &uart_cfg);

	if (err == -ENOSYS)
    {
		printk("Configuration is not supported by device or driver,"
			" check the UART settings configuration\n");
		return -ENOSYS;
	}

    // Initialize UART RX IRQ
    uart_irq_rx_disable(uart1_dev);
    uart_irq_tx_disable(uart1_dev);
    uart_irq_callback_user_data_set(uart1_dev, uart_cb, NULL);
    uart_irq_rx_enable(uart1_dev);

    while (1)
    {
        // sleep
        k_msleep(SLEEP_TIME_MS);
    }

    return ret;
}
