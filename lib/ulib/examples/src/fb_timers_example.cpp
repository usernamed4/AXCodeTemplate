/**
 * \file fb_timers_example.cpp
 * \brief Пример работы с функциональными блоками таймеров
 */

#include "plc/fb_debug.h"
#include "ulib/fb_timers.h"

using namespace plc;


/**
 * \brief Пример использования функционального блока \ref plc::TON
 * \details
 * Раз в заданный интервал времени выход таймера принимает значение true
 * на время одного цикла ПЛК и в терминал отправляется тестовое сообщение.
 * 
 * В данном примере интервал равен 1-ой секунде (параметр pt).
 */
void TON_example()
{
  static TON ton({
    .en = false,
    .pt = 1000
  });
  static bool start = false;
  
  ton.en = !start;
  ton();

  if (ton.q())
  {
    print_debug("TEST\n");
    start = true;
  }
  else
  {
    start = false;
  }
}


/**
 * \brief Пример использования функционального блока \ref plc::TOF
 * \details
 * Выполняет следующие действия:
 * - При отправке в консоль команды ON:
 *   - На вход таймера подается сигнал true
 *   - Выход принимает значение true
 *   - В консоль передаются значения:
 *     - Прошедшего времени (et)
 *     - Заданного времени  (pt)
 *     - Состояние выхода   (q)
 * 
 * При отправке команды OFF: 
 * - Вход таймера становится равен false
 * - Таймер начинает отсчет
 * - По истечении заданного времени выход становится равен false
 * */
void TOF_example()
{
  static TOF TOFtimer({
    .en = false,
    .pt = 10000
  });
  static bool on_state = false;
  static bool off_state = false;
  etl::string<10> string;
  
  scan_debug(string);

  if (string == "ON")
  {
    TOFtimer.en = true;
    on_state = true;
  }

  if (string == "OFF")
  {
    TOFtimer.en = false;
    off_state = true; 
  }

  TOFtimer(); 

  if (on_state)
  {
    print_debug(
      "[ET=%i,PT=%i,OUT=%i]\n", TOFtimer.et(), TOFtimer.pt, TOFtimer.q()
    );
    on_state = false;
  }

  if (off_state)
  {
    print_debug("[ET=%i,OUT=%i]\r\n", TOFtimer.et(), TOFtimer.q());

    if(!TOFtimer.q())
    {
      off_state = false;
    }
  }
}


/**
   * \brief Пример использования функционального блока \ref TP
   * \details
   * Каждые 5 секунд (pt) переменная out изменяет  значение с true на false 
   * и наоборот.  
   * В отладочную консоль выводится значение прошедшего времени и состояние
   * переменной out
   * */
void TP_example()
{
  static TP TPtimer({
    .en = false,
    .pt = 5000
  });
  static bool init = false;
  static bool out = false;

  if (!TPtimer.q() && !init)
  {
    TPtimer.en = true;
    init = true;
    out = !out;
  }

  TPtimer();

  if (!TPtimer.q() && init)
  {
    TPtimer.en = false;
    TPtimer();
    init = false; 
  }
  
  print_debug("[ET=%i,OUT=%i]\n", TPtimer.et(), out);
}
