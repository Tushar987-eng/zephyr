#include<zephyr/init.h>
#include<zephyr/kernel.h>

static int board_tushar_m3_scratch_init(void) {
    printk("Board Initialized\n");
    return 0;
}

SYS_INIT(board_tushar_m3_scratch_init, APPLICATION, 0);