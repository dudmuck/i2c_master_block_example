#include <stdint.h>
#include <linux/i2c-dev.h>

int i2c_setup(uint8_t sa, unsigned irqPin);
void i2c_service(void);

int write_test_buffer(const uint8_t*);
int read_test_buffer(uint8_t*);

extern int fd;
