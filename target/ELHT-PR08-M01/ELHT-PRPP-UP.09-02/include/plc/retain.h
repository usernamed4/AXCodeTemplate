/**
 * \file retain.h
 * \brief Retain-переменные для работы с EEPROM
 * \ingroup plc_retain
 */

#ifndef PLC_RETAIN_H
#define PLC_RETAIN_H

#include "plc/base.h"

namespace plc
{
  /**
   * \brief Макрос-квалификатор переменных для помещения их в EEPROM
   * \ingroup plc_retain
   */
  #define retain __attribute__((section (".eeprom"))) volatile

  //! Настройки retain
  struct RetainConfig
  {
    static uint8_t save_time; //!< Время сохранения в retain (в секундах)
  };

  /**
   * \brief Количество циклов записи в eeprom
   * 
   * \return Количество циклов записи в eeprom с момента изготовления прибора
   * \details Делит хранящийся в каждой странице счётчик на количество страниц
   * \ingroup plc_retain  
   */
  uint32_t GetNumberOfEepromWriteCycles();
}


#endif /* PLC_RETAIN_H */
