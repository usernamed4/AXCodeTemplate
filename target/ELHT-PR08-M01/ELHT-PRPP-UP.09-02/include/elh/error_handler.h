/**
 * \file elh/error_handler.h
 * \brief Обработчик ошибок
 * \ingroup common_errorHandler
 * \details
 */

#ifndef ELH_ERROR_HANDLER_H
#define ELH_ERROR_HANDLER_H

#include "etl/error_handler.h"
#include "elh/eventlog.h"
#include "etl/type_traits.h"

namespace elh
{
  /**
   * \brief Базовая структура мета-информации об ошибке/событии
   * \details
   * Для создания ошибки или события необходимо наследовать структуру от данной
   * и добавить следующие определения:
   * \code{.cpp}
   * using code = code<НОМЕР_ОШИБКИ>;
   * using is_error = etl::true_type или etl::false_type; (true если ошибка)
   * constexpr static const string_type text = "ТЕКСТОВЫЙ КОД ОШИБКИ";
   * \endcode
   */
  struct exception_helper
  {
    using string_type  = etl::exception::string_type;
    using numeric_type = etl::exception::numeric_type;

    //! Код ошибки
    template<numeric_type VALUE>
    using code = etl::integral_constant<numeric_type, VALUE>;
  };

  //! Пример ошибки
  struct example_exception : public exception_helper
  {
    using code     = code<0>;
    using is_error = etl::true_type;
    constexpr static const string_type text = "e0";
  };

  /**
   * \brief Шаблон обработчика ошибки/события
   * \details
   * - Если событие является ошибкой, добавляет сообщение в журнал ошибок
   * - Добавляет сообщение в журнал событий
   */
  template<typename T>
  etl::enable_if_t<etl::is_base_of_v<exception_helper, T>, void>
  event_handler(
    const exception_helper::string_type file_name,
    const exception_helper::numeric_type line_number
  ){
    if constexpr(T::is_error::value)
    {
      #ifdef ETL_LOG_ERRORS
      etl::error_handler::error({T::text, file_name, line_number});
      #endif
      eventlog::EventlogWrite(T::code::value, line_number);
    }
    else
    {
      eventlog::eventlog_record_t record = eventlog::EventlogWrite(
        T::code::value, line_number
      );
      eventlog::EventlogCallback(record);
    }
  }
}

/**
 * \addtogroup common_errorHandler
 * \{
 */


#if defined(ETL_NO_CHECKS)
  #define ELH_ASSERT_CUSTOM(b, e, f, l)             // Does nothing.
  #define ELH_ASSERT_CUSTOM_RETURN(b, e, f, l)      // Does nothing.
  #define ELH_ASSERT_CUSTOM_RETVAL(b, e, v, f, l)   // Does nothing.
  #define ELH_ASSERT_CUSTOM_FAIL(e, f, l)           // Does nothing.
  #define ELH_ASSERT_CUSTOM_FAIL_RETURN(e, f, l)    // Does nothing.
  #define ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, v, f, l) // Does nothing.
#elif ETL_USING_EXCEPTIONS
  #if defined(ETL_LOG_ERRORS)
    #define ELH_ASSERT_CUSTOM(b, e, f, l)           if (!(b)) {elh::event_handler<e>(f, l); throw((e));} // If the condition fails, calls the error handler then throws an exception.
    #define ELH_ASSERT_CUSTOM_RETURN(b, e, f, l)    if (!(b)) {elh::event_handler<e>(f, l); throw((e));} // If the condition fails, calls the error handler then throws an exception.
    #define ELH_ASSERT_CUSTOM_RETVAL(b, e, v, f, l) if (!(b)) {elh::event_handler<e>(f, l); throw((e));} // If the condition fails, calls the error handler then throws an exception.
    #define ELH_ASSERT_CUSTOM_FAIL(e, f, l)         elh::event_handler<e>(f, l); throw((e));            // Calls the error handler then throws an exception.
    #define ELH_ASSERT_CUSTOM_FAIL_RETURN(e, f, l)  elh::event_handler<e>(f, l); throw((e));            // Calls the error handler then throws an exception.
    #define ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, f, l)  elh::event_handler<e>(f, l); throw((e));            // Calls the error handler then throws an exception.
  #else
    #define ELH_ASSERT_CUSTOM(b, e, f, l)             if (!(b)) {throw((e));} // If the condition fails, throws an exception.
    #define ELH_ASSERT_CUSTOM_RETURN(b, e, f, l)      if (!(b)) {throw((e));} // If the condition fails, throws an exception.
    #define ELH_ASSERT_CUSTOM_RETVAL(b, e, v, f, l)   if (!(b)) {throw((e));} // If the condition fails, throws an exception.
    #define ELH_ASSERT_CUSTOM_FAIL(e, f, l)           throw((e));             // Throws an exception.
    #define ELH_ASSERT_CUSTOM_FAIL_RETURN(e, f, l)    throw((e));             // Throws an exception.
    #define ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, v, f, l) throw((e));             // Throws an exception.
  #endif
#else
  #if defined(ETL_LOG_ERRORS)
    #define ELH_ASSERT_CUSTOM(b, e, f, l)             if(!(b)) {elh::event_handler<e>(f, l);}            // If the condition fails, calls the error handler
    #define ELH_ASSERT_CUSTOM_RETURN(b, e, f, l)      if(!(b)) {elh::event_handler<e>(f, l); return;}    // If the condition fails, calls the error handler and return
    #define ELH_ASSERT_CUSTOM_RETVAL(b, e, v, f, l)   if(!(b)) {elh::event_handler<e>(f, l); return(v);} // If the condition fails, calls the error handler and return a value
    #define ELH_ASSERT_CUSTOM_FAIL(e, f, l)           elh::event_handler<e>(f, l);                       // Calls the error handler
    #define ELH_ASSERT_CUSTOM_FAIL_RETURN(e, f, l)    elh::event_handler<e>(f, l); return;               // Calls the error handler and return
    #define ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, v, f, l) elh::event_handler<e>(f, l); return(v); 
  #else
    #if ETL_IS_DEBUG_BUILD
      #define ELH_ASSERT_CUSTOM(b, e, f, l)             assert((b))   // If the condition fails, asserts.
      #define ELH_ASSERT_CUSTOM_RETURN(b, e, f, l)      assert((b))   // If the condition fails, asserts.
      #define ELH_ASSERT_CUSTOM_RETVAL(b, e, v, f, l)   assert((b))   // If the condition fails, asserts.
      #define ELH_ASSERT_CUSTOM_FAIL(e, f, l)           assert(false) // Asserts.
      #define ELH_ASSERT_CUSTOM_FAIL_RETURN(e, f, l)    assert(false) // Asserts.
      #define ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, v, f, l) assert(false) // Asserts.
    #else
      #define ELH_ASSERT_CUSTOM(b, e, f, l)                                  // Does nothing.
      #define ELH_ASSERT_CUSTOM_RETURN(b, e, f, l)      if (!(b)) return;    // Returns.
      #define ELH_ASSERT_CUSTOM_RETVAL(b, e, v, f, l)   if (!(b)) return(v); // Returns a value.
      #define ELH_ASSERT_CUSTOM_FAIL(e, f, l)                                // Does nothing.
      #define ELH_ASSERT_CUSTOM_FAIL_RETURN(e, f, l)    return;              // Returns.
      #define ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, v, f, l) return(v);           // Returns a value.
    #endif
  #endif
#endif

#define ELH_ASSERT(b, e)             ELH_ASSERT_CUSTOM(b, e, "", __LINE__)             // Проверка
#define ELH_ASSERT_RETURN(b, e)      ELH_ASSERT_CUSTOM_RETURN(b, e, "", __LINE__)      // Проверка, вернуть void если провалена
#define ELH_ASSERT_RETVAL(b, e, v)   ELH_ASSERT_CUSTOM_RETVAL(b, e, v, "", __LINE__)   // Проверка, вернуть значение если провалена
#define ELH_ASSERT_FAIL(e)           ELH_ASSERT_CUSTOM_FAIL(e, "", __LINE__)           // Проверка провалена
#define ELH_ASSERT_FAIL_RETURN(e)    ELH_ASSERT_CUSTOM_FAIL_RETURN(e, "", __LINE__)    // Проверка провалена, вернуть void
#define ELH_ASSERT_FAIL_RETVAL(e, v) ELH_ASSERT_CUSTOM_FAIL_RETVAL(e, v, "", __LINE__) // Проверка провалена, вернуть значение

/**
 * \}
 * \noop common_errorHandler
 */


#endif /* ELH_ERROR_HANDLER_H */
