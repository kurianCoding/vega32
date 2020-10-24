#include "config.h"
#include "gpio.h"
#include "interrupt.h"
#include "stdlib.h"
#include "uart.h"

global int pulsecount = 0;
void gpio_intr_handler(void) {
    pulsecount++;
    return;
}
int main() {
    irq_register_handler(GPIO_0_IRQ, gpio_intr_handler);
    interrupt_enable(GPIO_0_IRQ);
    clock_t startTime, endTime;
    long int rate = 0;

    startTime = read_csr(mcycle);
    for (; true;) {
	if (0.025 * (endTime - startTime) < 60) {
	    endTime = read_csr(mcycle);
	} else {
	    rate = pulsecount / 60;
	    pulsecount = 0;
	    startTime = 0;
	    endTime = 0;
	}
    }
    return;
}
