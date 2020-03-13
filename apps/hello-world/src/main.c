#include <stdio.h>

#include <sel4/sel4.h>
#include <simple/simple.h>
#include <simple-default/simple-default.h>
#include <sel4platsupport/platsupport.h>
#include <allocman/bootstrap.h>
#include <allocman/vka.h>
#include <sel4utils/vspace.h>

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

    error = platsupport_serial_setup_simple(&vspace, &simple, &vka);
    if (error) {
        ZF_LOGF("Failed to initialise serial port");
    }

    printf("Hello, World!\n");
    return 0;
}
