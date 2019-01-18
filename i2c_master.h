#include <stdint.h>
#include <linux/i2c-dev.h>

int i2c_setup(uint8_t sa, unsigned irqPin);
void i2c_service(void);

extern int fd;
