#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

/* Tegra X1 SoC Technical Reference Manual, version 1.3
 *
 * See Chapter 9 "Multi-Purpose I/O Pins", section 9.13 "GPIO Registers"
 * (table 32: GPIO Register Address Map)
 *
 * The GPIO hardware shares PinMux with up to 4 Special Function I/O per
 * pin, and only one of those five functions (SFIO plus GPIO) can be routed to
 * a pin at a time, using the PixMux.
 *
 * In turn, the PinMux outputs signals to Pads using Pad Control Groups. Pad
 * control groups control things like "drive strength" and "slew rate," and
 * need to be reset after deep sleep. Also, different pads have different
 * voltage tolerance. Pads marked "CZ" can be configured to be 3.3V tolerant
 * and driving; and pads marked "DD" can be 3.3V tolerant when in open-drain
 * mode (only.)
 *
 * The CNF register selects GPIO or SFIO, so setting it to 1 forces the GPIO
 * function. This is convenient for those who have a different pinmux at boot.
 */

using namespace std;

//  The only address we really need
#define GPIO_216      0x6000d60C    // Jetson Nano  7[AUDIO_MCLK]
#define GPIO_50       0x6000d108    // Jetson Nano 11[UART2_RTS]
#define GPIO_194      0x6000d600    // Jetson Nano 15[LCD_TE]
#define GPIO_14       0x6000d004    // Jetson Nano 13[SPI2_SCK]
#define GPIO_16       0x6000d008    // Jetson Nano 19[SPI1_MOSI]
#define GPIO_38       0x6000d100    // Jetson Nano 33[GPIO_PE6]
#define GPIO_77       0x6000d204    // Jetson Nano 38[I2S4_SDIN] // J

// From https://github.com/leahneukirchen/linux-jetson-tk1/blob/master/drivers/gpio/gpio-tegra.c
#define GPIO_INT_LVL_MASK		0x010101
#define GPIO_INT_LVL_EDGE_RISING	0x000101
#define GPIO_INT_LVL_EDGE_FALLING	0x000100
#define GPIO_INT_LVL_EDGE_BOTH		0x010100
#define GPIO_INT_LVL_LEVEL_HIGH		0x000001
#define GPIO_INT_LVL_LEVEL_LOW		0x000000

enum INOUT { 
    INPUT, OUTPUT=0xFF
};

//  layout based on the definitions above
//  Each GPIO controller has four ports, each port controls 8 pins, each
//  register is interleaved for the four ports, so
//  REGX: port0, port1, port2, port3
//  REGY: port0, port1, port2, port3
typedef struct {
    uint32_t CNF;
    uint32_t _padding1[3];
    uint32_t OE;
    uint32_t _padding2[3];
    uint32_t OUT;
    uint32_t _padding3[3];
    uint32_t IN;
    uint32_t _padding4[3];
    uint32_t INT_STA;
    uint32_t _padding5[3];
    uint32_t INT_ENB;
    uint32_t _padding6[3];
    uint32_t INT_LVL;
    uint32_t _padding7[3];
    uint32_t INT_CLR;
    uint32_t _padding8[3];
} gpio_t;

int main(void)
{
    //  read physical memory (needs root)
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("/dev/mem");
        fprintf(stderr, "please run this program as root (for example with sudo)\n");
        exit(1);
    }

    //  map a particular physical address into our address space
    int pagesize = getpagesize();
    int pagemask = pagesize-1;
    //  This page will actually contain all the GPIO controllers, because they are co-located
    void *base = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (GPIO_216 & ~pagemask));
    if (base == NULL) {
        perror("mmap()");
        exit(1);
    }

   //  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
    gpio_t volatile *pinLed = (gpio_t volatile *)((char *)base + (GPIO_216 & pagemask));

    pinLed->CNF = 0x00ff;
    pinLed->OE = OUTPUT;
    pinLed->OUT = 0xff;
    //  pin->IN = 0x00; read only
    //  disable interrupts
    pinLed->INT_ENB = 0x00;
    //  don't worry about these for now
    //pin->INT_STA[0] = 0x00;
    //pin->INT_LVL[0] = 0x000000;
    //pin->INT_CLR[0] = 0xffffff;

    fprintf(stderr, "press ctrl-C to stop\n");

    //  "blink" the output values
    uint8_t val = 0xff;
    while (true) {
        sleep(1);
        val = val ^ 0xff;
        pinLed->OUT = val;
	printf("%s \n","Hello!!!, My name is Hai");
    }

        /* turn off the LED */
    pinLed->OUT = 0;

    /* unmap */
    munmap(base, pagesize);

    /* close the /dev/mem */
    close(fd);

    printf("\nGood Bye!!!\n");
    return 0 ;
}
