/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>

int main(void)
{
	int i = 0;
    while(1) {
        k_sleep(K_MSEC(2000));
        printk("Hello World! %s %d\n", CONFIG_BOARD, i++);
    }
}