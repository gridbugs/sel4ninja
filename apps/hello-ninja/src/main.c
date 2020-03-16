#include <stdio.h>
#include <string.h>

#include <sel4/sel4.h>
#include <simple/simple.h>
#include <simple-default/simple-default.h>
#include <sel4platsupport/platsupport.h>
#include <sel4platsupport/io.h>
#include <allocman/bootstrap.h>
#include <allocman/vka.h>
#include <sel4utils/vspace.h>
#include <platsupport/chardev.h>

#include "uart.h"

#define ALLOCATOR_VIRTUAL_POOL_SIZE ((1 << seL4_PageBits) * 100)
static sel4utils_alloc_data_t data;

#define ALLOCATOR_STATIC_POOL_SIZE ((1 << seL4_PageBits) * 20)
static char allocator_mem_pool[ALLOCATOR_STATIC_POOL_SIZE];

int main(void) {
    int error;
    simple_t simple;

    seL4_BootInfo *info = platsupport_get_bootinfo();

    simple_default_init_bootinfo(&simple, info);

    allocman_t *allocman = bootstrap_use_current_simple(&simple, ALLOCATOR_STATIC_POOL_SIZE, allocator_mem_pool);
    if (allocman == NULL) {
        ZF_LOGF("Failed to create allocman");
    }

    vka_t vka;
    allocman_make_vka(&vka, allocman);

    vspace_t vspace;
    error = sel4utils_bootstrap_vspace_with_bootinfo_leaky(&vspace, &data, simple_get_pd(&simple), &vka, info);
    if (error) {
        ZF_LOGF("Failed to bootstrap vspace");
    }

    ps_io_ops_t ops;
    error = sel4platsupport_new_io_ops(&vspace, &vka, &simple, &ops);
    if (error) {
        ZF_LOGF("Failed to initialise io");
    }

    error = platsupport_serial_setup_simple(&vspace, &simple, &vka);
    if (error) {
        ZF_LOGF("Failed to initialise serial port");
    }
    printf("Hello, Ninja!\n");

    ps_chardevice_t chardev;
    if (ps_cdev_init(PS_SERIAL1, &ops, &chardev) == NULL) {
        ZF_LOGF("Failed to initialise PS_SERIAL1");
    }
    ninja_uart_init(&chardev);

    printf("Initialized serial device\n");

    char* msg;
    ssize_t len;
    ssize_t bytes_written;

    // Turn on eye LEDs
    msg = "{\"DEVICE\":[{\"G\":\"0\",\"V\":0,\"D\":1007,\"DA\":\"440044\"}]}";
    len = strlen(msg);
    printf("writing %s to uart (len=%u)\n", msg, len);
    bytes_written = ps_cdev_write(&chardev, msg, len, NULL, NULL);
    printf("wrote %d bytes (attempted %u)\n", bytes_written, len);

    for (volatile int i = 0; i < 10000000; i++);

    // Change status LED
    msg = "{\"DEVICE\":[{\"G\":\"0\",\"V\":0,\"D\":999,\"DA\":\"008080\"}]}";
    len = strlen(msg);
    printf("writing %s to uart (len=%u)\n", msg, len);
    bytes_written = ps_cdev_write(&chardev, msg, len, NULL, NULL);
    printf("wrote %d bytes (attempted %u)\n", bytes_written, len);


    return 0;
}
