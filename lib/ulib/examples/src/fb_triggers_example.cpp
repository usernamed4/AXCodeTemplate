/**
 * \file fb_triggers_example.cpp
 * \brief Пример работы с функциональными блоками триггеров
 */

#include "ulib/fb_triggers.h"
#include "plc/fb_debug.h"

using namespace plc;


/**
 * \brief Пример использования функционального блока \ref RTrig
 * \details
 * Выполняет следующие действия:
 * - Подает на вход блока за несколько циклов последовательность, выбираемую
 * из битовой маски переменной counter.  
 * Последовательность по умолчанию - 0, 0, 1, 1
 * - Выводит в консоль отладки текущее состояние входа, предыдущее
 * и выход блока
 * 
 * Чтобы изменить последовательность входов в примере, нужно внести изменения
 * в строку
 * \code{.cpp}
 * current_in = counter++ & 0x02;
 * \endcode
 * 
 * При замене 0x02 на 0x01 последовательность будет 0, 1, 0, 1.  
 * При замене 0x02 на 0x04 - 0, 0, 0, 0, 1, 1, 1, 1.
 */
void RTrig_example()
{
  static RTrig rtrig({
    .in = false
  });
  static uint8_t counter = 0;
  static bool last_in = false;
  bool current_in = false;
  bool current_q = false;

  last_in = current_in;
  current_in = counter++ & 0x02;

  rtrig.in = current_in;
  rtrig();
  current_q = rtrig.q();

  print_debug(
    "CURRENT=%i, LAST=%i, TRIG=%i\n", current_in, last_in, current_q
  );
}


  /**
 * \brief Пример использования функционального блока \ref FTrig
 * \param in Вход функционального блока
 * \details
 * В зависимости от длительности нахождения  переменной in в состоянии true
 * инвертирует состояния выходных переменных short_out и long_out.
 * 
 * Может использоваться для распознования короткого и длительного нажатия
 * кнопки. Если время нажатия counter меньше counter_sv то 
 * инвертируется short_out, если больше - то long_out.
 * 
 * Отсчет времени в данном примере задается в количествах циклов ПЛК.
 */
void FTrig_example(bool in)
{
  static FTrig ftrig({
    .in = false
  });
  static bool long_out = false;
  static bool short_out = false;
  static bool long_state = false;
  static uint16_t counter = 0;
  static uint16_t counter_sv = 50;
  static uint16_t counter_limit = 100;
  
  ftrig.in = in;
  ftrig();

  if (in)
  {
    counter++;

    if (counter > counter_limit)
    {
      counter = 0;
    }
  
    if (!long_state && (counter>counter_sv))
    {
      long_out = !long_out;
      long_state = true;
    }
  }

  if (ftrig.q())
  {
    if ((counter < counter_sv) && !long_state)
    {
      short_out = !short_out;
    }

    long_state = false;
    counter = 0;
  }

  print_debug(
    "IN=%i, SHORT_OUT=%i, LONG_OUT=%i, LONG_STATE=%i COUNTER=%i\n",
    in, short_out, long_out, long_state, counter
  );
}


/**
 * \brief Пример использования функционального блока \ref RSTrig
 * \details
 * Выводит в консоль отладки все возможные состояния RS-триггера
 * при различных комбинациях сигналов на входе. Состояния меняются при
 * каждом вызове функции примера.
 * */
void RSTrig_example()
{
  static RSTrig rstrig({
    .r = false,
    .s = false
  });
  static uint8_t state = 0;

  switch (state)
  {
    case 0: rstrig.r = false, rstrig.s = false; break;
    case 1: rstrig.r = false, rstrig.s = true;  break;
    case 2: rstrig.r = true,  rstrig.s = false; break;
    case 3: rstrig.r = true,  rstrig.s = true;  break;
  } 

  rstrig();
  print_debug("[RES=%i, SET=%i, Q=%i]\n", rstrig.r, rstrig.s, rstrig.q());

  if (state < 3)
  {
    state++;
  }
  else
  {
    state = 0;
  }
}


/**
 * \brief Пример использования функционального блока \ref SRTrig
 * \details
 * Выводит в консоль отладки все возможные состояния SR-триггера при различных
 * комбинациях сигналов на входе. Состояния меняются при каждом вызове функции
 * примера.
 */
void SRTrig_example()
{
  static SRTrig srtrig({
    .s = false,
    .r = false
  });
  static uint8_t state = 0;

  switch (state)
  {
    case 0: srtrig.s = false, srtrig.r = false; break;
    case 1: srtrig.s = false, srtrig.r = true;  break;
    case 2: srtrig.s = true,  srtrig.r = false; break;
    case 3: srtrig.s = true,  srtrig.r = true;  break;
  }

  srtrig();
  print_debug("[RES=%i, SET=%i, Q=%i]\n", srtrig.r, srtrig.s, srtrig.q());

  if (state<3)
  {
    state++;
  }
  else
  {
    state = 0;
  }
}
