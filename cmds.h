
#define CMD_TEST3               0x00
#define CMD_TEST12              0x01
#define CMD_TEST32              0x02
#define CMD_IRQ                 0x03

typedef union {
    struct __attribute__((packed)) {
        struct {
            uint8_t button  : 1;    // 0
            uint8_t xyz     : 1;    // 1
            uint8_t res     : 6;    // 2,3,4,5,6,7
        } flags;
        uint8_t cnt;     // if flags.button
        uint8_t a;
        int8_t b;
        int8_t c;
    } fields;
    uint8_t buf[5];
} irq_t;

extern const uint8_t cmd_to_length[];

