#include <platsupport/serial.h>

#define DLL         0x00
#define DLH         0x04
#define FCR         0x08
#define LCR         0x0C
#define MDR         0x20

#define REG_PTR(base, off)     ((volatile uint32_t *)((base) + (off)))

void ninja_uart_init(ps_chardevice_t* dev)
{
    *REG_PTR(dev->vaddr, MDR) = 7; // disable uart
    *REG_PTR(dev->vaddr, LCR) = 0x80; // config mode a
    *REG_PTR(dev->vaddr, FCR) = BIT(0); // set up fifo
    *REG_PTR(dev->vaddr, LCR) = 0xBF; // config mode b
    *REG_PTR(dev->vaddr, DLH) = 0x01; // 9.6k baud
    *REG_PTR(dev->vaddr, DLL) = 0x38; // 9.6k baud
    *REG_PTR(dev->vaddr, LCR) = BIT(0) | BIT(1); // 8-bit chars, operational mode
    *REG_PTR(dev->vaddr, MDR) = 0; // enable uart
}
