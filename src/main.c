/*
 * Copyright (c) 2024 Gemini Code Assist
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>

/* Use the shell UART instance by default */
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

LOG_MODULE_REGISTER(uart_async_sample, LOG_LEVEL_INF);

/* UART device struct */
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* Semaphore to signal TX completion */
static K_SEM_DEFINE(tx_done_sem, 1, 1);

/* Single byte buffer for transmission */
static uint8_t tx_buf;

/**
 * @brief UART Interrupt Service Routine
 *
 * @param dev The UART device structure
 * @param user_data User-defined data
 */
static void uart_isr(const struct device *dev, void *user_data)
{
	/* This ISR is called for every UART interrupt, so we check why it was called. */
	if (uart_irq_tx_ready(dev)) {
		/* The transmitter is ready to accept a new character.
		 * Since we are only sending one byte, we can now disable the TX interrupt.
		 */
		uart_irq_tx_disable(dev);

		/* Signal that the transmission is complete. */
		k_sem_give(&tx_done_sem);
	}
}

int main(void)
{
	int ret;

	/* Check if the UART device is ready */
	if (!device_is_ready(uart_dev)) {
		LOG_ERR("UART device not found!");
		return 0;
	}

	/* Set the ISR. This function will be called when a UART interrupt occurs. */
	uart_irq_callback_user_data_set(uart_dev, uart_isr, NULL);

	LOG_INF("UART Interrupt API sample started. Will send a random byte every 1s.");

	while (1) {
		/* Wait until the previous transmission is done */
		k_sem_take(&tx_done_sem, K_FOREVER);

		/* Generate a random byte */
		/* Use the low bits of the hardware cycle counter as a pseudo-random value */
		tx_buf = (uint8_t)k_cycle_get_32();

		LOG_INF("Sending byte: 0x%02x", tx_buf);

		/*
		 * 1. Enable the TX interrupt. The ISR will be called when the transmitter is empty.
		 * 2. Start the transmission. uart_tx will place the byte in the buffer and return.
		 */
		uart_irq_tx_enable(uart_dev);
		ret = uart_tx(uart_dev, &tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
		/* We don't check the return of uart_tx here as it's less critical in this flow */

		/* Wait for 1 second before the next transmission */
		k_sleep(K_SECONDS(1));
	}

	return 0;
}
