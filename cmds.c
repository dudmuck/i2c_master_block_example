#include <stdint.h>
#include "cmds.h"

const uint8_t cmd_to_length[] =
{
    /* 0x00 CMD_TEST3   */ 3,
    /* 0x01 CMD_TEST12  */ 12,
    /* 0x02 CMD_TEST32  */ 32,
    /* 0x03 CMD_IRQ     */ sizeof(irq_t)
};

