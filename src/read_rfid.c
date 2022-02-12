#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/watchdog.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define BUFFER_SIZE 14
uint8_t buf[BUFFER_SIZE];
uint8_t id[10];
uint8_t checksum[2];

void onReceiveHandler() {

  while (uart_is_readable(UART_ID)) {
    printf("\nTag present!\n");
    uart_read_blocking(UART_ID, buf, BUFFER_SIZE);

    int i;
    for (i=0; i<10; i++) {
      id[i] = buf[1+i];
    }
    for (i=0; i<2; i++) {
      checksum[i] = buf[11+i];
    }

    printf("ID: ");
    printf(id);
    printf("\nChecksum: ");
    printf(checksum);
  }

}

int main() {
  stdio_init_all();

  uart_init(UART_ID, BAUD_RATE);
  uart_set_hw_flow(UART_ID, false, false); // flow controls off
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  int UART_IRQ = UART0_IRQ;
  irq_set_exclusive_handler(UART_IRQ, onReceiveHandler);
  irq_set_enabled(UART_IRQ, true);
  uart_set_irq_enables(UART_ID, true, false);

  printf("Running...");

  while (true);
    tight_loop_contents();
  return 0;

}