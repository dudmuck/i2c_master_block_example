#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "i2c_master.h"
#include "cmds.h"

uint8_t irq_pin;
int fd;

void i2c_service()
{
    while (digitalRead(irq_pin)) {
        int result;
        irq_t irq;
        result = i2c_smbus_read_i2c_block_data(fd, CMD_IRQ, cmd_to_length[CMD_IRQ], irq.buf);
        if (result < 0) {
            perror("i2c_read_CMD_IRQ");
                usleep(200000);
        } else {
            if (irq.buf[0] == 0) {
                /* irq pin raised, but no pending irq: disconnected wire or slave not running */
                printf("\e[31mno irq\e[0m\n");
                usleep(200000);
                continue;
            }
            for (result = 0; result < cmd_to_length[CMD_IRQ]; result++)
                printf("%02x ", irq.buf[result]);
            if (irq.fields.flags.button) {
                printf("button:%u ", irq.fields.cnt);
            }
            printf("\n");
        }
    } // ..while irq pin asserted
}

int i2c_setup(uint8_t sa, unsigned irqPin)
{
    if (wiringPiSetup () < 0) {
        fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
        return -1;
    }
    pinMode(irqPin, INPUT);
    irq_pin = irqPin;

    fd = wiringPiI2CSetup(sa);
    if (fd < 0) {
        printf("i2c setup fail\n");
        return -1;
    }
    return 0;
}
