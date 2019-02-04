#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "i2c_master.h"
#include "cmds.h"

#define IRQ_PIN     7   /* wiringPi 7 = BCM GPIO 4 */

#define SLAVE_ADDRESS       0x50


int main(int argc, char* argv[])
{
    uint8_t test_buf[cmd_to_length[CMD_BUFFER]];
    int result;
	bool svc_irq = false;
	bool speed_test = false;
    time_t now, start;
    unsigned nbytes;

    if (i2c_setup(SLAVE_ADDRESS, IRQ_PIN) < 0) {
        printf("i2c_setup fail\n");
        return -1;
    }

    if (argc < 2) {
        printf("%s i                        wait for IRQ\n", argv[0]);
        printf("%s [rw] <3 or 12 or 32>	 [n_reps]	read / write\n", argv[0]);
        printf("%s st                       speed test\n", argv[0]);
        return -1;
    }

    if (argv[1][0] == 'r' || argv[1][0] == 'w') {
        unsigned rep, i, test, cmd, len, reps = 1;
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

        if (argv[3]) {
            sscanf(argv[3], "%u", &reps);
        }

        for (rep = 0; rep < reps; rep++) {
            if (argv[1][0] == 'w') {
                for (i = 0; i < len; i++)
                    buf[i] = i;

                result = i2c_smbus_write_i2c_block_data(fd, cmd, len, buf);
                if (result < 0)
                    perror("test write");
                else
                    printf("%u wrote %d\n", rep, result);
            } else {
                result = i2c_smbus_read_i2c_block_data(fd, cmd, len, buf);
                if (result < 0) {
                    perror("i2c_smbus_read_i2c_block_data");
                    return -1;
                }
                printf("%u test read %u: ", rep, result);
                for (i = 0; i < len; i++)
                    printf("%02x ", buf[i]);
                printf("\n");
            }
        }
    } else if (argv[1][0] == 'i') {
        svc_irq = true;
    } else if (argv[1][0] == 's' && argv[1][1] == 't') {
        time_t start = time(NULL);
        nbytes = 0;
        srand(start);
        svc_irq = true;
        speed_test = true;
        for (now = time(NULL); now == start;)
            now = time(NULL);
    }

	if (svc_irq) {
        printf("service irq..\n");
		for (;;) {
    		i2c_service();
            if (speed_test) {
                time_t now = time(NULL);
                if (now == start) {
                    /* same second */
                    int len;
                    uint8_t buf[cmd_to_length[CMD_BUFFER]];
                    len = read_test_buffer(buf);
                    if (len < 0) {
                        printf("read fail\n");
                        speed_test = false;
                    } else {
                        if (memcmp(buf, test_buf, sizeof(test_buf)) != 0) {
                            printf("memcmp fail\n");
                            speed_test = false;
                        } else
                            nbytes += len;
                    }
                } else {
                    /* new second */
                    unsigned n, stop = sizeof(test_buf) >> 2;
                    uint32_t* u32ptr;
                    u32ptr = (uint32_t*)test_buf;
                    for (n = 0; n < stop; n++)
                        *u32ptr++ = rand();
                    /*for (n = 0; n < 32; n++)
                        printf("%02x ", test_buf[n]);
                    printf("\n");*/
                    if (write_test_buffer(test_buf) < 0)
                        speed_test = false;

                    start = now;
                    printf("%u bytes per second\n", nbytes);
                    nbytes = sizeof(test_buf);
                }
            } else
			    usleep(50000);
		} // ..for (;;)
	} // ..if (svc_irq)

    close(fd);
}

