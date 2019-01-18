#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "i2c_master.h"
#include "cmds.h"

#define IRQ_PIN     7   /* wiringPi 7 = BCM GPIO 4 */

#define SLAVE_ADDRESS       0x50


int main(int argc, char* argv[])
{
    int result;
	bool svc_irq = false;

    if (i2c_setup(SLAVE_ADDRESS, IRQ_PIN) < 0) {
        printf("i2c_setup fail\n");
        return -1;
    }

    if (argc < 2) {
        printf("%s i                        wait for IRQ\n", argv[0]);
        printf("%s [rw] <3 or 12 or 32>		read / write\n", argv[0]);
        return -1;
    }

    if (argv[1][0] == 'r' || argv[1][0] == 'w') {
        unsigned i, test, cmd, len;
        uint8_t buf[32];
        if (argc < 3) {
rwhelp:
            printf("usage:%s %s <3 or 12 or 32>\n", argv[0], argv[1]);
            return -1;
        }
        sscanf(argv[2], "%u", &test);
        switch (test) {
            case 3: cmd = CMD_TEST3; break;
            case 12: cmd = CMD_TEST12; break;
            case 32: cmd = CMD_TEST32; break;
            default: goto rwhelp;
        }
        len = cmd_to_length[cmd];
        if (argv[1][0] == 'w') {
            for (i = 0; i < len; i++)
                buf[i] = i;

            result = i2c_smbus_write_i2c_block_data(fd, cmd, len, buf);
            if (result < 0)
                perror("i2c_smbus_write_i2c_block_data");
            else
                printf("wrote %d\n", result);
        } else {
            result = i2c_smbus_read_i2c_block_data(fd, cmd, len, buf);
            if (len < 0) {
                perror("i2c_smbus_read_i2c_block_data");
                return -1;
            }
            printf("test read %u: ", result);
            for (i = 0; i < len; i++)
                printf("%02x ", buf[i]);
            printf("\n");
        }
    } else if (argv[1][0] == 'i') {
        svc_irq = true;
    }

	if (svc_irq) {
        printf("service irq..\n");
		for (;;) {
    		i2c_service();
			usleep(50000);
		}
	}
}

