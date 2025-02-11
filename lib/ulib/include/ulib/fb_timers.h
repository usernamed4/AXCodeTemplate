/**
 * \file ulib/fb_timers.h
 * \brief Функциональные блоки таймеров
 * \ingroup ulib_fb_timers
 */

#pragma once

#include "plc/fb_base.h"

namespace plc
{
  /**
   * \addtogroup ulib_fb_timers
   * \{
   */

  /**
   * \brief Функциональный блок таймера задержки включения
   */
  class TON : public FunctionalBlock
  {
  public:
    //! Разрешение работы
    bool en;
    //! Уставка времени, мс
    uint32_t pt;

    //! Выход завершения отсчета
    bool q() const;

    //! Осталось времени, мс
    uint32_t rt() const;

    //! Прошло времени, мс
    uint32_t et() const;

  protected: 
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Разрешение работы
      bool en {false};
      //! Уставка времени, мс
      uint32_t pt {0};
    };
    
  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    TON(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    TON();

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _q;
    bool _mem;
    uint32_t _et;
    uint32_t _rt;
    uint32_t _start_time;
  };


  /**
   * \brief Функциональный блок таймера задержки выключения
   */
  class TOF : public FunctionalBlock
  {
  public:
    //! Разрешение работы
    bool en;
    //! Уставка времени, мс
    uint32_t pt;

    //! Выход завершения отсчета
    bool q() const;

    //! Осталось времени, мс
    uint32_t rt() const;

    //! Прошло времени, мс
    uint32_t et() const;

    protected: 
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Разрешение работы
      bool en {false};
      //! Уставка времени, мс
      uint32_t pt {0};
    };
    
  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    TOF(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    TOF();

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _q;
    bool _mem;
    uint32_t _et;
    uint32_t _rt;
    uint32_t _start_time;
  };


  /**
   * \brief Функциональный блок импульса включения заданной длительности
   * \details
   * Выход включается по фронту на входе en на заданное время pt.  
   * После включения состояние выхода не зависит от изменения сигнала на входе.
   */ 
  class TP : public FunctionalBlock
  {
  public:
    //! Разрешение работы
    bool en;
    //! Уставка времени, мс
    uint32_t pt;

    //! Выход завершения отсчета
    bool q() const;

    //! Осталось времени, мс
    uint32_t rt() const;

    //! Прошло времени, мс
    uint32_t et() const;

  protected: 
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Разрешение работы
      bool en {false};
      //! Уставка времени, мс
      uint32_t pt {0};
    };
    
  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    TP(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    TP(); 

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _q;
    bool _mem;
    uint32_t _et;
    uint32_t _rt;
    uint32_t _start_time;
  };

  /**
   * \}
   * \noop ulib_fb_timers
   */

}
