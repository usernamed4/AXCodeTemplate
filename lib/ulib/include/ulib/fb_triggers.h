/**
 * \file ulib/fb_triggers.h
 * \brief Функциональные блоки триггеров
 * \ingroup ulib_fb_triggers
 */

#pragma once

#include "plc/fb_base.h"


namespace plc
{
  /**
   * \addtogroup ulib_fb_triggers
   * \{
   */

  /**
   * \brief Триггер переднего фронта
   * \details
   * Активирует выход ровно на один цикл POU при появлении сигнала на входе
   * в текущем цикле и отсутствии сигнала в предыдущем
   */
  class RTrig : public FunctionalBlock
  {
  public:
    //! Вход триггера
    bool in;

    //! Выход триггера
    bool q() const;

  protected:
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Вход триггера
      bool in {false};
    };

  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    RTrig(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    RTrig();

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _mem;
    bool _q;
  };


  /**
   * \brief Триггер заднего фронта
   * \details
   * Активирует выход ровно на один цикл POU при пропадании сигнала на входе
   * в текущем цикле и наличии сигнала в предыдущем
   */
  class FTrig : public FunctionalBlock
  {
  public:
    //! Вход триггера
    bool in;

    //! Выход триггера
    bool q() const;

  protected:
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Вход триггера
      bool in {false};
    };

  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    FTrig(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    FTrig();

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _mem;
    bool _q;
  };


  /**
   * \brief RS-триггер c приоритетом отключения
   * \details
   * Активирует выход при поступлении сигнала на вход s, выключает выход при
   * поступлении сигнала на вход r. При наличии сигнала на обоих входах выход
   * отключен
   */
  class RSTrig : public FunctionalBlock
  {
  public:
    //! Вход RESET
    bool r;
    //! Вход SET
    bool s;

    //! Выход триггера
    bool q() const;

  protected: 
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Вход RESET
      bool r {false};
      //! Вход SET
      bool s {false};
    };
    
  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    RSTrig(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    RSTrig();

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _q;
  };


  /**
   * \brief SR-триггер c приоритетом включения
   * \details
   * Активирует выход при поступлении сигнала на вход s, выключает выход при
   * поступлении сигнала на вход r. При наличии сигнала на обоих входах выход
   * включен
   */
  class SRTrig : public FunctionalBlock
  {
  public:
    //! Вход SET
    bool s;
    //! Вход RESET
    bool r;

    //! Выход триггера
    bool q() const;

  protected: 
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Вход SET
      bool s {false};
      //! Вход RESET
      bool r {false};
    };

  public:
    /**
     * \brief Конструктор функционального блока
     * \param init Структура инициализации входных параметров
     */
    SRTrig(const Init& init);

    /**
     * \brief Конструктор функционального блока с параметрами по умолчанию
     */
    SRTrig();

    //! Выполняет функциональный блок
    void operator()();

  private:
    bool _q;
  };

 /**
   * \}
   * \noop ulib_fb_triggers
   */







}



