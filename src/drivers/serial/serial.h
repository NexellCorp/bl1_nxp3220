/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __SERIAL_H__
#define __SERIAL_H__

/* Function Define */
 void serial_set_baudrate (int channel, int uclk, int baud_rate);
 int serial_init(unsigned int channel);

char serial_getc(void);
void serial_putc(char ch);

 int serial_is_uart_tx_done(void);
 int serial_is_tx_empty(void);
 int serial_is_busy(void);

#endif	//#ifndef __SERIAL_H__
