/**
 * \file eventlog_table.h
 * \brief Распределение списка всех событий и ошибок процессорного модуля
 */

#pragma once

#define USER_EVENTS_CODE         (0)
#define CPU_EVENTS_CODE          (100)
#define KERNEL_EVENTS_CODE       (200)
#define PLC_EVENTS_CODE          (300)
#define CPU_EXCEPTION_CODE       (1000)
#define MODBUS_EXCEPTION_CODE    (1200)
#define DEVICE_EXCEPTION_CODE    (1300)
#define ELH_EXCEPTION_CODE       (1400)
#define TERMINAL_EXCEPTION_CODE  (1500)
#define TIME_MENU_EXCEPTION_CODE (1600)
#define MENU_EXCEPTION_CODE      (1700)
#define KERNEL_EXCEPTION_CODE    (2000)
#define MCU_EXCEPTION_CODE       (3000)
#define PLC_EXCEPTION_CODE       (4000)
