/**
 * \file system_api.h
 * \brief Список системных определений Alpha-X CPU 01-1 00
 */

#ifndef SYSTEM_API_H
#define SYSTEM_API_H

#include <cstdint>
#include "device.h"
#include "system_info.h"


//! Модификация ЭЛХТ-ПР08-М.01
#define ELHT_PR08_M01 (true)
//! Артикул модификации ЭЛХТ-ПР08-М.01 (Alpha-X CPU 01-1 00)
#define ALPHA_X_CPU_01_1_00 (ELHT_PR08_M01)

//! Список COM-портов модификации
enum COM_PORT : uint8_t
{
  COM1,
  COM2,
  COM3,
  COM_USB,
  COM_SIZE
};

//! Нумерация дискретных входов в устройстве \ref cpu_io
enum CPU_DINPUT
{
  CPU_DINPUT1,    //!< Дискретный вход №1
  CPU_DINPUT2,    //!< Дискретный вход №2
  CPU_DINPUT3,    //!< Дискретный вход №3
  CPU_DINPUT4,    //!< Дискретный вход №4
  CPU_DINPUT_SIZE //!< Кол-во дискретных входов
};

//! Нумерация дискретных выходов в устройстве \ref cpu_io
enum CPU_DOUTPUT
{
  CPU_DOUTPUT1,    //!< Дискретный выход №1
  CPU_DOUTPUT2,    //!< Дискретный выход №2
  CPU_DOUTPUT_SIZE //!< Кол-во дискретных выходов
};

//! Устройство собственных входов/выходов
extern device::iDevice& cpu_io;

#endif /* SYSTEM_API_H */
