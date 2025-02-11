/**
 * \file ulib/example/fb_etrig_example.h
 * \brief Пример создания функционального блока типа Execute с входом abort
 * на основе класса \ref ETrigA
 */

#include "ulib/example/fb_etriga_example.h"


namespace plc::example
{
  ExampleETrigA::ExampleETrigA(const Init& init)
  : input_a   {init.input_a},
    input_b   {init.input_b},
    _input_a  {false},
    _input_b  {0},
    _output_a {false},
    _output_b {0}
  {
    ;
  }


  /**
   * \details
   * Возвращает значение выходного параметра a. Метод не позволяет изменять 
   * значение выхода в самом функциональном блоке
   */
  bool ExampleETrigA::output_a() const
  {
    return _output_a;
  }


  /**
   * \details
   * Возвращает значение выходного параметра b. Метод не позволяет изменять 
   * значение выхода в самом функциональном блоке
   */
  int ExampleETrigA::output_b() const
  {
    return _output_b;
  }


  /**
   * \brief Реализация пользовательской логики функционального блока
   * \details
   * Вызывается в состоянии \ref plc::FB_STATE::EXECUTING при каждом
   * выполнении функционального блока, если для блока сброшен входной
   * параметр \ref abort. Если пользователь установит входной параметр
   * abort, то данный метод не будет вызван.
   * 
   * Копирование входных параметров во внутренние происходит по условию
   * метода \ref isFirstInvocation(). Данный метод возвращает true только
   * при первом вызове блока после перехода в состояние 
   * \ref plc::FB_STATE::EXECUTING. После сброса блока и запуска заново метод
   * снова вернет true в первом цикле работы.
   * 
   * Метод использует встроенную переменную \ref _sub_state для задержки 
   * выполнения блока. При первом вызове переменная равна нулю и увеличивается
   * на один. При следующем вызове по условию _sub_state == 1 происходит 
   * назначение выходов блока согласно описанию его работы в классе.
   * 
   * Если выставить входной параметр input_b равным 100 или больше, метод 
   * переведет блок в состояние ошибки неправильных входных параметров
   * \ref FB_ERROR_CODE_INVALID_PARAMETERS. Номер ошибки можно считать с помощью
   * метода \ref error_id(), когда блок находится в состоянии ошибки.
   * Для установки ошибки используется метод \ref returnError(). Метод запишет
   * в блок номер ошибки из аргумента и вернет состояние ошибки
   * 
   * \return Следующее состояние функционального блока. Любая реализация 
   * данного метода может возвращать только приведенные ниже значения
   * (иначе выдаст ошибку)
   * \retval plc::FB_STATE::EXECUTING Продолжает свое выполнение
   * \retval plc::FB_STATE::DONE Блок выполнен
   * \retval plc::FB_STATE::ERROR Произошла ошибка выполнения блока
   */
  FB_STATE ExampleETrigA::processExecuting()
  {
    if(isFirstInvocation())
    {
      _input_a = input_a;
      _input_b = input_b;
    }

    if(_input_b >= 100)
    {
      return returnError(FB_ERROR_CODE_INVALID_PARAMETERS);
    }

    if(_sub_state == 0)
    {
      _sub_state = 1;
      return FB_STATE::EXECUTING;
    }
    else
    {
      _output_a = !_input_a;
      _output_b = _input_b + 1;
      return FB_STATE::DONE;
    }
  }


  /**
   * \brief Сбрасывает значение выходных параметров при установке пользователем
   * параметра \ref abort во время выполнения блока
   * \details
   * Метод реализован для примера
   * 
   * Обычно реализация данного метода не требуется, потому что выходные 
   * параметры в большинстве случаев имеют верное значение только в состояниях
   * \ref plc::FB_STATE::DONE или \ref plc::FB_STATE::ERROR (в некоторых 
   * случаях)
   * 
   * \retval Следующее состояние функционального блока. Любая реализация 
   * данного метода может возвращать только приведенные ниже значения
   * (иначе выдаст ошибку)
   * \retval plc::FB_STATE::ABORTING Сброс продолжается (не завершен)
   * \retval plc::FB_STATE::ABORTED  Сброс выполнен
   * \retval plc::FB_STATE::ERROR    Произошла ошибка сброса
   */
  FB_STATE ExampleETrigA::processAborting()
  {
    _output_a = false;
    _output_b = 0;
    return FB_STATE::ABORTED;
  }


  /**
   * \brief Сбрасывает значение выходных параметров при сбросе блока
   * (выполнение блока с execute == false после завершения его работы)
   * \details
   * Метод реализован для примера. Метод не может возвращать ошибку, т.к.
   * должен обнулить блок
   * 
   * Обычно реализация данного метода не требуется, потому что выходные 
   * параметры в большинстве случаев имеют верное значение только в состояниях
   * \ref plc::FB_STATE::DONE или \ref plc::FB_STATE::ERROR (в некоторых 
   * случаях)
   * 
   * \retval Следующее состояние функционального блока. Любая реализация 
   * данного метода может возвращать только приведенные ниже значения
   * (иначе выдаст ошибку)
   * \retval plc::FB_STATE::RESETTING Сброс продолжается (не завершен)
   * \retval plc::FB_STATE::DORMANT   Сброс выполнен
   */
  FB_STATE ExampleETrigA::processResetting()
  {
    _output_a = false;
    _output_b = 0;
    return FB_STATE::DORMANT;
  }
}
