# I2C master block transfer example
Using [raspberry Pi](https://en.wikipedia.org/wiki/Raspberry_Pi) I2C with [STM32](https://www.st.com/en/ecosystems/stm32-nucleo.html?querycriteria=productId=SC2003) as slave, with block transfers up to 32 byte size.

This example operates with slave [i2c_slave_block_example](https://os.mbed.com/users/dudmuck/code/i2c_slave_block_example/)

Raspberry Pi I2C driver doesnt support SMBUS variable length read, so I2C block transfer is used.  Block length is mapped to command byte.

# build instructions
for raspberry pi
* [install wiringPi](http://wiringpi.com/download-and-install/) library
* ``sudo apt install libi2c-dev``(has i2c block data functions)
* ``mkdir build``
* ``cd build``
* ``cmake`` ..
* ``make``

# wiring connections
example: NUCLEO board as slave:
| signal name      | slave (nucleo) pin    | RPi pin (master)  |
| ------------- |:-------------:| -----:|
| GND      |  | 6 |
| SCL      | D15      |  5 |
| SDA | D15      |  3 |
| IRQ | D6      |  4  (wiringPi 7) |

See [wiring pi pin mapping](http://wiringpi.com/pins/) for raspberry pi.

NUCLEO board connection can be on CN10 header, inner row.

# test
```$ ./test_i2c r 32
test read 32: 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 
$ ./test_i2c r 12
test read 12: 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 
$ ./test_i2c i
service irq..
01 00 00 00 00 button:0 
01 01 00 00 00 button:1```

