/**
 * \file fb_base.h
 * \brief Базовый интерфейс функциональных блоков
 */

#pragma once

#include "plc/base.h"

namespace plc
{
  //! Тип данных для перечисления ошибок
  using error_t = uint32_t;

  //! Состояния функциональных блоков типа Execute и Enable
  enum class FB_STATE
  {
    DORMANT,   //!< Состояние ожидания выполнения блока
    EXECUTING, //!< Состояние выполнения блока
    DONE,      //!< Состояние успешного завершения выполнения блока
    ERROR,     //!< Состояние ошибки блоки
    ABORTING,  //!< Состояние остановки выполнения блока
    ABORTED,   //!< Состояние остановленного блока
    RESETTING, //!< Состояние сброса блока
    SIZE       //!< Кол-во состояний FB_STATE
  };

  //! Стандартные ошибки функционального блока
  enum FB_ERROR_CODE : error_t
  {
    //! Нет ошибки
    FB_ERROR_CODE_NONE               = 0,
    //! Вышло время выполнения блока (таймаут)
    FB_ERROR_CODE_TIMEOUT            = 1,
    //! Неправильные входные параметры
    FB_ERROR_CODE_INVALID_PARAMETERS = 2,
    //! Слишком много запросов к ядру от функциональных блоков
    FB_ERROR_CODE_TOO_MANY_REQUESTS  = 3,
    //! Системный сбой
    FB_ERROR_CODE_INTERNAL_FAIL      = 4,
    //! Для ФБ не реализован обработчик EXECUTING
    FB_ERROR_CODE_NO_EXECUTE_HANDLER = 5,
    //! Неправильный переход в конечном автомате ФБ
    FB_ERROR_CODE_WRONG_TRANSITION   = 6
  };


  /**
   * \brief Базовая структура функционального блока
   * \details
   * Используется компилятором для автоматического определения классов
   * функциональных блоков в программе
   */
  struct FunctionalBlock
  {
  };


  /**
   * \brief Базовый интерфейс функционального блока типа Execute
   * \details
   * Содержит всю логику ФБ типа Execute согласно модели функциональных блоков
   * PLCopen.
   * 
   * Для реализации блока на основе интерфейса необходимо наследовать один из 
   * классов ФБ типа Execute из списка ниже и реализовать функцию обработки
   * состояния \ref FB_STATE::EXECUTING - \ref processExecuting().
   * 
   * Классы для наследования при создании собственных функциональных блоков:
   * - \ref ETrig
   * - \ref ETrigTo
   * - \ref ETrigATo
   * - \ref EtrigA
   * 
   * \todo Добавить ФБ типа Execute с Time Limit
   * 
   * \ref processExecuting() может вернуть:
   * - \ref FB_STATE::EXECUTING для продолжения выполнения блока в следующем
   * цикле
   * - \ref FB_STATE::DONE для окончания выполнения блока
   * - \ref FB_STATE::ERROR при определении пользовательской ошибки выполнения
   * блока. При этом должен быть установлен error_id (рекомендуется использовать
   * метод \ref returnError())
   * 
   * Дополнительные входные параметры помещаются в public-секцию класса.
   * 
   * Выходные параметры должны быть методами, возвращающими внутренние
   * значения переменных
   * 
   * При запуске исполнения блока входные параметры должны быть скопированы
   * в непубличную секцию чтобы изменение входных параметров не влияло
   * на выполнение блока.
   * Для определения первого запуска выподнения блока в состоянии 
   * \ref FB_STATE::EXECUTING используется метод \ref isFirstInvocation()
   * 
   * \todo Подробнее работа блока расписана в документации в разделе
   * \ref plc_fb_section_plcopen_execute.
   */
  class FBExecute : public FunctionalBlock
  {
  private:
    FB_STATE _state    {FB_STATE::DORMANT};  //!< Состояние ФБ
    error_t  _error_id {0};                  //!< Номер ошибки
    bool     _first_invocation {true};       //!< Первый запуск EXECUTING
    uint32_t _timeout    {0};                //!< Время таймаута
    uint32_t _start_time {}; //!< Время первого запуска выполнения блока

  protected:
    //! Возможные входы блока
    struct Inputs
    {
      bool     execute {false}; //!< Вход выполнения блока
      bool     abort   {false}; //!< Вход сброса блока
      uint32_t timeout {0};     //!< Вход таймаута блока
      uint8_t  flags;           //!< Системные флаги функционального блока
    };

    uint32_t _sub_state {0}; //!< Вспомогательное состояние выполнения блока

    //! Выход успешного выполнения блока
    bool __done() const  { return _state == FB_STATE::DONE; }
    //! Выход выполнения функции
    bool __busy() const  { return _state == FB_STATE::EXECUTING; }
    //! Выход остановки выполнения блока
    bool __aborted() const { return _state == FB_STATE::ABORTED; }
    //! Выход бездействия блока
    bool __idle() const { return _state == FB_STATE::DORMANT; }
    //! Выход состояния ошибки
    bool __error() const { return _state == FB_STATE::ERROR; }
    //! Выход кода ошибки
    error_t __error_id() const { return _error_id; }

    //! Проверка на первый запуск блока в EXECUTING
    bool isFirstInvocation() const { return _first_invocation; }

    /**
     * \brief Установка ошибки блока
     * \details
     * Возвращает состояние \ref STATE::ERROR и устанавливает 
     * ошибку \ref error_id.
     * 
     * Номера ошибок должны соответствовать \ref FB_ERROR_CODE и списку ошибок,
     * дополнительно установленных для используемого функционального блока.
     * 
     * Типовое использованеи метода - совместно с инструецией return для
     * выхода из метода обработки функционального блока
     * \code{.cpp}
     * return returnError(FB_ERROR_CODE);
     * \endcode
     */
    FB_STATE returnError(error_t error_id); 

    //! Обработчик состояния EXECUTING
    virtual FB_STATE processExecuting() =0;
    //! Обработчик состояния RESETTING
    virtual FB_STATE processResetting();
    //! Обработчик состояния ABORTING
    virtual FB_STATE processAborting();

    //! Метод выполнения блока
    void __handler(const Inputs&);

  };


  /**
   * \brief ФБ типа Execute
   * \details
   * Для создания собственных ФБ на основе блока необходимо:
   * - Наследовать класс нового ФБ от текущего
   * \code{.cpp}
   * class YourName : public plc::ETrig
   * \endcode
   * - Добавить дополнительные входы и выходы
   * - Реализовать методы обработчиков состояний ФБ:
   * \code{.cpp}
   * plc::FB_STATE processExecuting(); // ОБЯЗАТЕЛЬНО
   * plc::FB_STATE processResetting(); // При необходимости
   * plc::FB_STATE processAborting();  // При необходимости
   * \endcode
   * 
   * \todo Закончить инструкцию по добавлению пользовательских ФБ
   */
  class ETrig : public FBExecute
  {
  public:
    //! Вход выполнения блока
    bool execute {false};

    //! Выход успешного выполнения блока
    bool done() const        { return __done(); }
    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    //! Метод выполнения ФБ
    void operator()();
  };


  /**
   * \brief ФБ типа Execute с входом TimeOut
   */
  class ETrigTo : public FBExecute
  {
  public:
    //! Вход выполнения блока
    bool execute {false};
    //! Вход максимального времени выполнения блока (таймаута), мс
    uint32_t timeout {1000};

    //! Выход успешного выполнения блока
    bool done() const        { return __done(); }
    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    void operator()();
  };

  /**
   * \brief ФБ типа Execute с входами TimeOut и Abort
   */
  class ETrigATo : public FBExecute
  {
  public:
    //! Вход выполнения блока
    bool execute     {false};
    //! Вход сброса выполнения блока
    bool abort       {false};
    //! Вход максимального времени выполнения блока (таймаута), мс
    uint32_t timeout {1000};

    //! Выход успешного выполнения блока
    bool done() const        { return __done(); }
    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход сброса выполнения блока
    bool aborted() const     { return __aborted(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    //! Метод выполнения ФБ
    void operator()();
  };


  /**
   * \brief ФБ типа Execute с входом Abort
   */
  class ETrigA : public FBExecute
  {
  public:
    //! Вход выполнения блока
    bool execute {false};
    //! Вход сброса выполнения блока
    bool abort   {false};

    //! Выход успешного выполнения блока
    bool done() const        { return __done(); }
    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход сброса выполнения блока
    bool aborted() const     { return __aborted(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    //! Метод выполнения ФБ
    void operator()();
  };


  /**
   * \brief Базовый интерфейс функционального блока типа Enable
   * \details
   * Содержит всю логику ФБ типа Enable согласно модели функциональных блоков
   * PLCopen.
   * 
   * Для реализации блока на основе интерфейса необходимо наследовать один из
   * классов ФБ типа Enable из списка ниже и реализовать функцию обработки
   * состояния \ref FB_STATE::EXECUTING - \ref processExecuting().
   * 
   * Классы для наследования при создании собственных функциональных блоков:-
   * - \ref LCon
   * - \ref LConTo
   * - \ref LConC
   * 
   * \todo Добавить ФБ типа Enable с Time Limit
   * 
   * \ref processExecuting() может вернуть:
   * - \ref FB_STATE::EXECUTING для продолжения выполнения блока в следующем
   * цикле
   * - \ref FB_STATE::DONE для окончания выполнения блока (кроме ФБ \ref LConC)
   * - \ref FB_STATE::ERROR при определении пользовательской ошибки выполнения
   * блока. При этом должен быть установлен error_id с помощью метода
   * \ref returnError())
   * 
   * Дополнительные входные параметры помещаются в public-секцию класса.
   * 
   * Выходные параметры должны быть методами, возвращающими внутренние
   * значения переменных.
   * 
   * Входные переменные не должны копироваться в непубличную секцию
   * и должны использоваться напрямую при каждом выполнении блока.
   * 
   * \todo Подробнее работа блока расписна в документации в разделе
   * \ref plc_fb_section_plcopen_enable.
   */
  class FBEnable : public FBExecute
  {
  };


  /**
   * \brief Функциональный блок типа Enable
   * \details
   * Для создания собственных ФБ на основе блока необходимо:
   * - Наследовать класс нового ФБ от текущего
   * \code{.cpp}
   * class YourName : public plc::LCon
   * \endcode
   * - Добавить дополнительные входы и выходы
   * - Реализовать методы обработчиков состояний ФБ:
   * \code{.cpp}
   * plc::FB_STATE processExecuting(); // ОБЯЗАТЕЛЬНО
   * plc::FB_STATE processResetting(); // Крайне рекомендуется для сброса входов
   * plc::FB_STATE processAborting();  // При необходимости
   * \endcode
   * 
   * \todo Закончить инструкцию по добавлению пользовательских ФБ
   */
  class LCon : public FBEnable
  {
  public:
    bool enable {false};

    //! Выход успешного выполнения блока
    bool done() const        { return __done(); }
    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    //! Метод выполнения ФБ
    void operator()();
  };


  /**
   * \brief ФБ типа Enable с входом TimeOut
   */
  class LConTo : public FBEnable
  {
  public:
    //! Вход выполнения блока
    bool     enable  {false};
    //! Вход максимального времени выполнения блока (таймаута), мс
    uint32_t timeout {1000};

    //! Выход успешного выполнения блока
    bool done() const        { return __done(); }
    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    //! Метод выполнения ФБ
    void operator()();
  };


  /**
   * \brief ФБ типа Enable без завершения выполнения блока
   * (Continuous Behaviour)
   */
  class LConC : public FBEnable
  {
  public:
    //! Вход выполнения блока
    bool enable {false};

    //! Выход выполнения блока
    bool busy() const        { return __busy(); }
    //! Выход бездействия блока
    bool idle() const        { return __idle(); }
    //! Выход ошибки блока
    bool error() const       { return __error(); }
    //! Выход номера ошибки блока
    error_t error_id() const { return __error_id(); }

    //! Метод выполнения ФБ
    void operator()();
  };
}
