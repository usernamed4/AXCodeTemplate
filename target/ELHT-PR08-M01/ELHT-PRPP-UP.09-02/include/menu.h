/**
 * \file menu.h
 * \brief Библиотека меню (таблицы параметров)
 */

#ifndef MENU_H
#define MENU_H

#include <cstdint>
#include "etl/array.h"
#include "etl/span.h"
#include "etl/vector.h"
#include "etl/delegate.h"
#include "elh/access.h"

namespace menu
{
  // Прототип класса
  class iTable;

  //! Флаги переменных
  namespace flag
  {
    //! Типы флагов параметров
    enum TYPE
    {
      VISIBLE = 1 << 0, //!< Видимый параметр
      EDIT    = 1 << 1, //!< Редактируемый параметр
      SAVE    = 1 << 2, //!< Сохраняется в EEPROM
      RESET   = 1 << 3  //!< Может сбрасываться на заводские настройки
    };
  }

  using flag_t = uint16_t;


  //! Тип значения переменной для чтения
  enum class DATA_T
  {
    VALUE,   //!< Текущее значение
    DEFAULT, //!< Значение по-умолчанию
    MIN,     //!< Минимальное значение
    MAX      //!< Максимальное значение
  };

  //! Cтатус сохранения переменной в таблицу меню
  enum class SET_STATUS
  {
    OK,    //!< Сохранено успешно
    WRONG, //!< Попытка записи недопустимого значения
    ERROR  //!< Ошибка сохранения
  };


  /**
   * \brief Настройки записи переменной в меню
   */
  struct SetAttributes
  {
    enum SELECT_VARIABLES
    {
      SELECT_VARIABLES_ALL,
      SELECT_VARIABLES_UPDATED,
      SELECT_VARIABLES_UNCHANGED
    };

    //! Проверять диапазон допустимых значений
    bool check_value{true};
    //! Проверять флаг редактирования параметра
    bool check_edit{true};
    //! Тип задействованных переменных
    SELECT_VARIABLES select_variables{SELECT_VARIABLES_ALL};   
  };


  /**
   * \brief Интерфейс работы с переменными меню
   * \details
   * 
   */
  class ivariable {
  protected:
    const size_t _size; //!< Размер значения переменной (в байтах)

    constexpr ivariable(const size_t size, flag_t flags)
    : _size{size},
      flags{flags}
    {}

  public:
    /**
     * \brief Флаги переменной
     * \todo Следует заменить на более удобный вариант задания флагов через
     * битовые поля структуры. Текущий вариант задания флагов закладывался 
     * на случай если программа пользователя будет заливаться независимо от
     * ядра, а ядро бы шифровалось. В таком случае нам нужно было гарантировать
     * порядок флагов в переменной чтобы при изменении настроек компилятор
     * не мог поменять переменные местами и рассинхронизировать значения флагов
     * в ядре и в программе пользователя
     */
    flag_t flags;

    /**
     * \brief Возвращает размер значения переменной (в байтах)
     */
    const size_t size() const { return _size; }

    /**
     * \brief Проверяет указанное значение переменной на допустимость
     */
    virtual bool is_allowed_by_ptr(const void* from) const =0;

    /**
     * \brief Возвращает указатель на требуемый тип данных 
     */
    virtual const void* get_data(const DATA_T data_type) const =0;

    /**
     * \brief Возвращает флаги переменной
     */
    const flag_t getFlags() const { return flags; }
  };


  /**
   * \brief Переменная меню
   * \tparam T Тип переменной языка программирования, оборачиваемый в 
   * переменную меню
   * \details
   * \note Переменные меню должны иметь модификатор const чтобы не занимать 
   * место в оперативной памяти.
   * 
   * Позволяет обращаться в таблице меню к своему значению через собственное 
   * имя.
   * 
   * Содержит:
   * - Размер переменной
   * - Ссылку на структуру со всеми данными переменной для работы с меню
   * 
   * Не хранит в себе значение переменной. Значение каждой переменной лежит
   * в массиве значений таблицы меню. В таблице меню так же хранится адрес 
   * данной переменной.
   */
  template<typename T>
  struct var : public ivariable
  {
    using type = T; //!< Тип переменной

    //! Тип функции дополнительной проверки допустимых значений
    using check_value_function_t = etl::delegate<bool(T)>;

    //! Структура инициализации переменной
    struct InitData
    {
      //! Флаги переменной
      flag_t flags {flag::VISIBLE | flag::EDIT | flag::RESET};
      //! Заводское значение переменной
      T default_value {};
      //! Минимальное значение переменной
      T min_value {etl::numeric_limits<T>::lowest()};
      //! Максимальное значение переменной
      T max_value {etl::numeric_limits<T>::max()};
      //! Дополнительная функция проверки допустимых значений переменной
      check_value_function_t check_value_f{};
    };

    //! Заводское значение переменной
    T default_value;

    //! Минимальное значение переменной
    T min_value;

    //! Максимальное значение переменной
    T max_value;

    /**
     * \brief Дополнительная функция проверки допустимых значений переменной
     * \details
     * Применяется после проверки на минимальное и максимальное значение,
     * если задана
     */
    check_value_function_t check_value_f;


    /**
     * \brief Конструктор с заданными параметрами переменной
     */
    constexpr var(const InitData& init)
    : ivariable(sizeof(T), init.flags),
      default_value{init.default_value},
      min_value{init.min_value},
      max_value{init.max_value},
      check_value_f{init.check_value_f}
    {
    }


    /**
     * \brief Конструтор с параметрами переменной по умолчанию
     * \details Указывает параметры \ref default_data
     */
    constexpr var()
    : var{InitData{}}
    {
    }


    /**
     * \brief Проверяет допустимость указанного значения для переменной
     * \param new_value Проверяемое значение
     * \return true, если значение допустимо
     */
    bool isAllowedValue(T new_value) const
    {
      if(new_value < min_value)
      {
        return false;
      }
      if(new_value > max_value)
      {
        return false;
      }

      return (check_value_f.is_valid()) ? check_value_f(new_value) : true;
    }


    /**
     * \brief Проверяет на допустимость значение переменной по указателю
     * \param from Указатель на проверяемое значение переменной
     * \details
     * Используется для проверки без знания о типе переменной (например,
     * при обходе меню из переменных разных типов в цикле)
     * \todo Можно поменять void* на T* для большей безопасности при 
     * написании кода
     */
    bool is_allowed_by_ptr(const void* from) const override final
    {
      T new_value;
      memcpy(&new_value, from, sizeof(T));
      return isAllowedValue(new_value);
    }


    /**
     * \brief Возвращает указатель на требуемый тип данных 
     * \details
     * Метод позволяет брать информацию о переменной, не зная ее конкретного
     * типа.
     * 
     * Используется, например, в обходе таблицы меню через цикл.
     * 
     * \todo Можно поменять void* на T* для большей безопасности при 
     * написании кода
     */
    const void* get_data(const DATA_T data_type) const override final
    {
      switch(data_type)
      {
        default:              return nullptr;
        case DATA_T::VALUE:   return nullptr;
        case DATA_T::DEFAULT: return &default_value;
        case DATA_T::MIN:     return &min_value;
        case DATA_T::MAX:     return &max_value;
      }
    }
  };


  /**
   * \brief Указатель на переменную в конкретном меню
   * \details
   * 
   * 
   * \attention Менять значения структуры вручную запрещено
   * \todo Запретить менять значения структуры на уровне кода
   * 
   * \todo Можно добавить сюда методы работы с меню и работать с переменной
   * через методы вида iTable::at() и iTable[]
   */
  struct TableVariable
  {
    //! Указатель на таблицу меню
    iTable* table;

    //! Указатель на переменную
    const ivariable* variable;

    //! Индекс переменной в таблице меню
    size_t position;
  };

  //! Оператор равенства указателей на переменную в конкретном меню
  bool operator==(const TableVariable&, const TableVariable&);
  //! Оператор неравенства указателей на переменную в конкретном меню
  bool operator!=(const TableVariable&, const TableVariable&);


  /**
   * \brief Интерфейс описания таблицы меню
   * \details
   * Хранит список переменных.
   * 
   * Описание задается отдельно от таблицы чтобы поместить его в FLASH-память.
   * 
   * \todo Добавить расчет адресов переменных на этапе компиляции и убрать их
   * из оперативной памяти в классе \ref Table<T>
   */
  class iTableInfo
  {
  public:
    //! Тип переменной, используемый для хранения адресов значений переменных
    using size_type = uint16_t;

    //! Диапазон указателей на массив используемых переменных меню
    const etl::span<const ivariable* const> variables;
    // const etl::span<const size_type> values_address;

  protected:
    /**
     * \brief Конструктор интерфейса описания таблицы меню
     * \param variables Указатель на список переменных таблицы меню
     */
    constexpr iTableInfo(
      const etl::span<const ivariable* const> variables
    )
    : variables{variables}
    {
    }
  };


  /**
   * \brief Описание таблицы меню
   * \tparam T Список базовых типов переменных для переменных меню, заданный
   * в порядке следования. Список подтягивается автоматически из конструктора
   * описания таблицы меню.
   * \details
   * \note Объекты описания таблицы меню должны иметь модификатор const чтобы 
   * не занимать место в оперативной памяти.
   * 
   * Пример описания таблицы меню из трех переменных:
   * \code{.cpp}
   * menu::var<int16_t> var_i16;
   * menu::var<int32_t> var_i32;
   * menu::var<float>   var_f;
   * 
   * menu::TableInfo info {
   *   var_i16,
   *   var_i32,
   *   var_f
   * };
   * \endcode
   * 
   * \todo Придумать как рассчитывать адреса переменных в буфере на этапе 
   * компиляции
   * \todo Собирать одну TableInfo из нескольких на этапе компиляции
  */
  template<typename... T>
  class TableInfo : public iTableInfo
  {
  public:
    enum
    {
      //! Количество переменных в описании таблицы меню
      VARS_COUNT = sizeof...(T), 
      //! Кол-во байт, занимаемых значениями переменных из описания таблицы меню
      VALUE_BYTES = (sizeof(T) + ... + 0)
    };

    static_assert(VARS_COUNT > 0, "Empty TableInfo");

    /**
     * \brief Конструктор описания таблицы меню
     * \param variables Список переменных меню в порядке следования
     */
    constexpr TableInfo(const var<T>&... variables, ...)
    : iTableInfo{_variables_storage},
      _variables_storage{&variables...}
    {
    }

    //! Создание пустого описания запрещено
    constexpr TableInfo() = delete;

  private:
    //! Хранилище указателей на используемые переменные меню
    const etl::array<const ivariable* const, VARS_COUNT> _variables_storage;
  };


  /**
   * \brief Интерфейс таблицы меню
   */
  class iTable
  {
  public:
    //! Тип размера переменных меню
    using size_type = iTableInfo::size_type;
    //! Тип битовой маски изменившихся переменных
    using update_mask_t = uint32_t;

  protected:
    //! Псевдоним типа данных размером 1 байт
    using byte_t = uint8_t;

    //! Псевдоним типа данных массива переменных типа T размером N
    template<typename T, size_t N>
    using array = etl::array<T, N>;

    //! Псевдоним типа данных диапазона указателей на массив переменных типа T
    template<typename T>
    using span = etl::span<T>;

    //! Структура инициализации интерфейса меню
    struct Init {
      //! Кол-во переменных в меню
      const size_type size;
      //! Описание таблицы меню
      const iTableInfo& info;
      //! Диапазон массива адресов значений переменных
      const span<size_type> values_address;
      //! Диапазон массива значений переменных
      const span<byte_t> values;
      //! Диапазон битовых масок флагов изменившихся переменных
      const span<update_mask_t> updated_variables_group;
      //! Мьютекс таблицы параметров
      elh::mutex* mutex;
    };

  public:
    //! Структура считывания маски изменившихся переменных
    struct UpdatedMaskData
    {
      update_mask_t mask{};        //!< Маска флагов изменившихся переменных
      size_type start_position{};  //!< Первая переменная в маске
      size_type variables_count{}; //!< Кол-во переменных в маске
    };

    //! Переменная для возврата ошибочных значений
    static const ivariable& dummy_variable;
    
    //! Таблица-пустышка для возврата в случае ошибок
    static iTable& dummy_table;

    //! Описание таблицы меню
    const iTableInfo& info;

  protected:
    //! Диапазон указателей на массив адресов значений переменных
    const span<size_type> _values_address;

    //! Диапазон указателей на массив значений переменных
    const span<byte_t> _values;

    //! Диапазон указателей на флаги изменившихся переменных
    const span<update_mask_t> _updated_variables_group;

    enum CONST
    {
      //! Кол-во флагов изменившихся переменных в \ref update_mask_t
      UPDATE_MASK_SIZE = sizeof(update_mask_t) * 8
    };

    /**
     * \brief Конструктор интерфейса таблицы меню
     * \param size Кол-во переменных в меню
     * \param info Описание таблицы меню
     * \param values_address Диапазон указателей на массив адресов значений 
     * переменных
     * \param values Указатель на массив значений переменных
     */
    constexpr iTable(const Init& init)
    : info                     {init.info},
      _values_address          {init.values_address},
      _values                  {init.values},
      _updated_variables_group {init.updated_variables_group},
      _size                    {init.size},
      _mutex                   {init.mutex}
    {
    }

  public:

    /**
     * \brief Возвращает количество переменных в таблице
     */
    size_type size() const { return _size; }

    /**
     * \brief Возвращает кол-во байт для хранения значений переменных в таблице
     */
    constexpr size_t size_bytes() const { return _values.size(); }

    /**
     * \brief Запрос на блокировку меню
     * \details
     * Блокирует таблицу параметров, если для таблицы указан мьютекс.
     * 
     * \retval true Таблица успешно заблокирована или не имеет мьютекса
     * \retval false Таблица занята другим процессом
     */
    bool lock(unsigned int time);

    /**
     * \brief Разблокировка меню
     * \details
     * Производит разблокировку таблицы параметров, если для не указан мьютекс.
     * 
     * \retval true Таблица успешно разблокирована или не имеет мьютекса
     * \retval false Произошла ошибка разблокировки
     */
    bool unlock();

    /**
     * \brief Ищет первое вхождение переменной в меню
     * \return Индекс переменной в таблице
     * \return \ref size(), если переменной нет в таблице
     */
    size_type find(const ivariable& variable) const;

    /**
     * \brief Сбрасывает значения переменных на заводские
     * \param force_reset если false, сбрасывает только переменные с флагом
     * resetable, иначе - все
     */
    void resetToDefault(const bool force_reset=false);

    /**
     * \brief Возвращает значение переменной из таблицы меню
     * \tparam T Тип значения переменной
     * \param variable Считываемая переменная
     * \details
     * 
     * Пример использования:
     * \code{.cpp}
     * const menu::var<int16_t> var_i16;
     * const menu::TableInfo info {
     *   var_i16
     * };
     * menu::Table example_table {info};
     * 
     * void func()
     * {
     *   int16_t x;
     *   x = example_table.get(var_i16);
     * }
     * \endcode
     */
    template<typename T>
    T get(const var<T>& variable) const { return get<T>(find(variable)); }

    /**
     * \brief Возвращает значение переменной по указанному номеру
     * \tparam T Тип значения переменной
     * \param position Номер переменной в таблице меню (начиная с 0)
     * \details
     * Пример использования:
     * \code{.cpp}
     * const menu::var<int16_t> var_i16;
     * const menu::var<int32_t> var_i32;
     * const menu::var<float>   var_f;
     * const menu::TableInfo info {
     *   var_i16,
     *   var_i32,
     *   var_f
     * };
     * menu::Table example_table {info};
     * 
     * void func()
     * {
     *   int32_t x;
     *   // Считывание значения переменной меню var_i32 в переменную x
     *   // (нумерация в порядке указания переменных в переменной info)
     *   x = example_table.get(1);
     * }
     * \endcode
     */
    template<typename T>
    T get(const size_t position) const
    {
      T answer{};
      get_by_ptr(position, &answer, sizeof(T));
      return answer;
    }

    /**
     * \brief Записывает значение переменной в таблицу меню
     * \param variable Сохраняемая переменная
     * \param new_value Новое значение переменной
     * \details
     * Перед записью метод проверяет попадает ли новое значение в диапазон
     * допустимых значений из описания типа переменной.
     * 
     * В случае успешной записи (значение переменной отличается от старого
     * и обновлено) будет вызывана функция \ref user::SaveCallback.
     * 
     * Пример использования:
     * \code{.cpp}
     * const menu::var<int16_t> var_i16;
     * const menu::TableInfo info {
     *   var_i16
     * };
     * menu::Table example_table {info};
     * 
     * void func()
     * {
     *   example_table.set(var_i16, 153);
     * }
     * \endcode
     * 
     * \attention Недопустимо использовать данный метод (с аргументом var<T>) 
     * для сохранения значения в меню, где используется несколько переменных
     * меню с одинаковым названием. В таком случае вместо аргумента var<T>
     * следует использовать \ref set<T>(size_t position, T new_value).
     * 
     * \return \ref SET_STATUS::OK если значение переменной обновлено
     * \return \ref SET_STATUS::WRONG если подано недопустимое значение
     * переменной
     * \return \ref SET_STATUS::ERROR если произошла системная ошибка
     */
    template<typename T>
    SET_STATUS set(
      const var<T>& variable, T new_value, const SetAttributes& attrib={}
    ){
      return set(find(variable), new_value, attrib);
    }

    /**
     * \brief Записывает значение переменной из таблицы параметров
     * \param variable Сохраняемая переменная
     * \param new_value Новое значение переменной
     * \details
     * Версия \ref set(const var<T>& variable, T new_value) с преобразованием 
     * в T значения других типов
     * \return \ref SET_STATUS::OK если значение переменной обновлено
     * \return \ref SET_STATUS::WRONG если подано недопустимое значение
     * переменной
     * \return \ref SET_STATUS::ERROR если произошла системная ошибка
     * \todo Добавить проверку шаблона if_castable или что-то в таком духе
     * для проверки на этапе компиляции
     */
    template<typename T, typename Y>
    SET_STATUS set(
      const var<T>& variable, Y new_value, const SetAttributes& attrib={}
    ){
      return set(find(variable), static_cast<T>(new_value), attrib);
    }

    /**
     * \brief Записывает значение переменной из таблицы параметров
     * \param position Номер переменной в таблице меню (начиная с 0)
     * \param new_value Новое значение переменной
     * \details
     * Версия \ref set(const var<T>& variable, T new_value) с указанием номера
     * переменной вместо её имени.
     * 
     * Пример использования:
     * \code{.cpp}
     * const menu::var<int16_t> var_i16;
     * const menu::var<int32_t> var_i32;
     * const menu::var<float>   var_f;
     * const menu::TableInfo info {
     *   var_i16,
     *   var_i32,
     *   var_f
     * };
     * menu::Table example_table {info};
     * 
     * void func()
     * {
     *   // Сохранение значения 154 в переменную var_i32 
     *   // (нумерация в порядке указания переменных в переменной info)
     *   example_table.set(1, 154);
     * }
     * \endcode
     * 
     * \return \ref SET_STATUS::OK если значение переменной обновлено
     * \return \ref SET_STATUS::WRONG если подано недопустимое значение
     * переменной
     * \return \ref SET_STATUS::ERROR если произошла системная ошибка
     */
    template<typename T>
    SET_STATUS set(size_t position, T new_value, const SetAttributes& attrib={})
    {
      return set_by_ptr(position, &new_value, attrib);
    }

    /**
     * \brief Копирует значение переменной в область памяти по указателю
     * \param variable Считываемая перменная
     * \details
     * Используется для чтения значения переменной:
     * - Для обхода переменных меню в цикле
     * - Если тип переменной неизвестен заранее
     * 
     * \attention Недопустимо использовать данный метод (с аргументом var<T>) 
     * для сохранения значения в меню, где используется несколько переменных
     * меню с одинаковым названием.  
     * В таком случае вместо аргумента var<T> следует использовать 
     * \ref get_by_ptr<T>(size_type position, void* to, const DATA_T data_type).
     */
    bool get_by_ptr(
      const ivariable& variable,
      void* to,
      size_type to_size,
      const DATA_T data_type=DATA_T::VALUE
    ) const;

    /**
     * \brief Копирует значение переменной в область памяти по указателю
     * \details
     * Используется для чтения значения переменной:
     * - Для обхода переменных меню в цикле
     * - Если тип переменной неизвестен заранее
     */
    bool get_by_ptr(
      size_type position,
      void* to,
      size_type to_size,
      const DATA_T data_type=DATA_T::VALUE
    ) const;


    /**
     * \brief Возвращает константный указатель на значение переменной в меню
     */
    const void* getValuePtr(size_type position) const;

    /**
     * \brief Записывает значение по указателю в переменную
     * \return \ref SET_STATUS::OK если значение переменной обновлено
     * \return \ref SET_STATUS::WRONG если подано недопустимое значение
     * переменной
     * \return \ref SET_STATUS::ERROR если произошла системная ошибка
     * \note Ошибки записи регистрируются в логе ошибок
     */
    SET_STATUS set_by_ptr(
      const ivariable& variable,
      const void* from,
      const SetAttributes& attrib={}
    );

    /**
     * \brief Записывает значение по указателю в переменную
     * \return \ref SET_STATUS::OK если значение переменной обновлено
     * \return \ref SET_STATUS::WRONG если подано недопустимое значение
     * переменной
     * \return \ref SET_STATUS::ERROR если произошла системная ошибка
     * \note Ошибки записи регистрируются в логе ошибок
     */
    SET_STATUS set_by_ptr(
      size_type position, const void* from, const SetAttributes& attrib={}
    );

    /**
     * \brief Возвращает маску изменившихся переменных в группе из 32 переменных
     * \param group Номер группы переменных (адреса group*32...group+1/32)
     * \param inverted Инвертирует маску (возвращает неизменившиеся переменные)
     * \details
     * Метод используется для оптимизации скорости обхода таблицы параметров.
     * 
     * Метод не устанавливает ошибку при выходе за размер меню, а устанавливает
     * 0 в количестве переменных в возвращаемом значении.
     * 
     * После проверки всех переменных следует вызвать метод
     * \ref clearUpdatedVariableGroupMask() или
     * \ref clearAllUpdatedVariablesMasks() для очистки списка изменившихся 
     * переменных.
     * 
     * \return Пара из двух элементов: маска изменившихся переменных и кол-во
     * переменных в маске
     */
    UpdatedMaskData getUpdatedVariablesMask(
      uint16_t group, bool inverted={false}
    ) const;

    /**
     * \brief Проверяет изменилось ли значение переменной
     * \details
     * После проверки всех переменных следует вызвать метод
     * \ref clearVariableChangedGroups() для очистки списка изменившихся
     * переменных.
     */
    bool isVariableUpdated(size_type position);

    /**
     * \brief Очищает флаг изменившейся переменной
     */
    void clearUpdatedVariableFlag(size_type position);

    /**
     * \brief Очищает флаги изменившихся переменных в группе из 32 переменных
     */
    void clearUpdatedVariableGroupMask(size_type group);

    /**
     * \brief Очищает флаги всех изменившихся переменных в меню
     */
    void clearAllUpdatedVariablesMasks();

    /**
     * \brief Возвращает переменную по адресу со смещением и проч.
     */
    TableVariable at(size_type position);

    /**
     * \brief Сравнивает одинаковые переменные в другой таблице меню
     * \details Источником переменных является текущее меню.
     * Сложность алгоритма O(N*other.N)
     * 
     * Для каждой переменной текущего меню должно быть найдено соответствие
     * переменных
     * \return true, если найдены одна или больше одинаковых переменных,
     * и их значения совпадают
     * \return false если хотя бы одна одинаковая переменная не совпадает
     * по значению
     * \return false если нет совпадающих переменных (также вызывает ошибку)
     * \attention Неприменимо если в меню есть несколько одинаковых переменных
     */
    bool isSameVariablesEqual(const iTable& other) const;


    /**
     * \brief Проверяет тип таблицы
     * \param[in] info Описание типа таблицы
     * \return true если таблица соответствует описанию
     * \return false если таблица не совпадает по описанию
     */
    bool isSameType(const iTableInfo& info) const;


    /**
     * \brief Сравнивает типы двух таблиц
     * \param[in] other Сравниваемая таблица
     * \return true если таблицы одного типа и размера
     * \return false если таблицы разного типа и/или размера
     */
    bool isSameType(const iTable& other) const;


    /**
     * \brief Копирование таблицы меню
     * \details
     * Позволяет копировать значения в таблицы с одинаковыми переменными.
     * 
     * В случае попытки копирования в таблицу с другими переменными вызовет
     * ошибку.
     * \param other Копируемая таблица
     * \param attrib Настройки копирования переменных
     * \return Кол-во незаписанных переменных
     */
    size_type copyTable(const iTable& other, const SetAttributes& attrib={});

    /**
     * \brief Копирует совпадающие переменные из другой таблицы меню
     * \param other Таблица с копируемыми переменными
     * \param attrib Настройки копирования переменных
     * \attention Неприменимо если в меню есть несколько одинаковых переменных
     * \return Кол-во незаписанных переменных
    */
    size_type copySameVariables(
      const iTable& other, const SetAttributes& attrib={}
    );

    /**
     * \brief Возвращает переменную-пустышку для ошибочных значений
     * \details
     * Указывает на переменную \ref dummy_variable c индексом \ref size()
     */
    TableVariable getDummyVariable();

    /**
     * \brief Перегрузка копирования таблицы меню
     * \details Выполняет \ref copyTable() с аргументом force
     */
    iTable& operator=(const iTable& other);

    friend bool operator==(const iTable& left, const iTable& right);
    friend bool operator!=(const iTable& left, const iTable& right);

  protected:
    /**
     * \brief Рассчитывает адреса переменных в буфере значений
     */
    void calculate_values_address();

  private:
    const size_type _size; //!< Кол-во переменных в меню
    elh::mutex* const _mutex{nullptr};

    SET_STATUS __set_by_ptr(size_type, const void*, const SetAttributes& a={});
    void* get_value_ptr(size_type) const;
    const ivariable* const get_variable(size_type) const;
    size_type get_value_address(size_type) const;
  };


  /**
   * \brief Оператор сравнения значений таблиц
   * \details Сравнивает таблицы на совпадение по переменным, а затем на
   * совпадение по значениям
   * \return true если значения переменных полностью совпадают
   * \return false если хотя бы одно значение не совпадает
   * \return false если сравниваются таблицы с разными параметрами
   * (также вызовет ошибку)
   */
  bool operator==(const iTable& left, const iTable& right);


  /**
   * \brief Оператор сравнения значений таблиц по несовпадению
   * \return Инверсно \ref operator==(const iTable&, const iTable&)
   */
  bool operator!=(const iTable& left, const iTable& right);


  /**
   * \brief Таблица параметров
   * \tparam N_VAR Кол-во переменных в таблице
   * \tparam INFO Описание таблицы параметров
   */
  template<size_t N_VAR, typename INFO>
  class Table : public iTable 
  {
  public:
    constexpr Table(const INFO& info, elh::mutex* mutex=nullptr)
    : iTable{{
        .size                    {N_VAR},
        .info                    {info},
        .values_address          {_values_address},
        .values                  {_values_storage},
        .updated_variables_group {_changed_variables_group_storage},
        .mutex                   {mutex}
      }}
    {
      iTable::calculate_values_address();
      iTable::resetToDefault(true);
      iTable::clearAllUpdatedVariablesMasks();
    }

    //! Копирование таблицы меню
    Table& operator=(const iTable& other)
    {
      iTable::operator=(other);
      return *this;
    }

  private:
    iTable::array<iTable::size_type, INFO::VARS_COUNT> _values_address;
    iTable::array<
      iTable::byte_t,
      etl::conditional_integral_constant<
        INFO::VARS_COUNT==1, size_t, INFO::VALUE_BYTES*N_VAR, INFO::VALUE_BYTES
      >::value
    > _values_storage;
    iTable::array<
      iTable::update_mask_t,
      (N_VAR / iTable::UPDATE_MASK_SIZE)
      + ((N_VAR % iTable::UPDATE_MASK_SIZE) ? 1 : 0)
    > _changed_variables_group_storage;
  };

  template<typename... T>
  Table(const TableInfo<T...>&) ->
    Table<TableInfo<T...>::VARS_COUNT, TableInfo<T...>>;

  template<typename... T>
  Table(const TableInfo<T...>&, elh::mutex*) ->
    Table<TableInfo<T...>::VARS_COUNT, TableInfo<T...>>;

  // Прототип класса дерева меню
  class iTableTree;


  /**
   * \brief Интерфейс узла дерева меню (таблицы параметров)
   * \details
   * Может хранить в себе несколько узлов и/или несколько таблиц меню
   */
  class iTableNode
  {
  protected:
    /**
     * \brief Конструктор узла дерева меню по начальному адресу и списку
     * потомков (child nodes)
     * \param nodes Список узлов-потомков
     * \param start_address Смещение адресов потомков
    */
    constexpr iTableNode(
      etl::span<const iTableNode*> nodes,
      size_t start_address
    )
    : _nodes{nodes},
      _start_address{start_address}
    {
    }


    /**
     * \brief Конструктор листа дерева с таблицей меню и начальным адресом
     * (без потомков)
     * \param tables Список таблиц меню в узле
     * \param start_address Смещение адресов таблиц меню в узле
    */
    constexpr iTableNode(
      const etl::span<iTable*> tables,
      size_t start_address
    )
    : _tables{tables},
      _start_address{start_address}
    {
    }

    friend iTableTree;

  public:
    //! Узлы-потомки в дереве
    etl::span<const iTableNode*> _nodes{};
    //! Таблицы меню в узле
    etl::span<iTable*> _tables{};
    //! Смещение адреса таблицы параметров в узле
    size_t _start_address;
  };


  /**
   * \brief Узел дерева меню с таблицами
   * \tparam TTables Список таблиц меню
   */
  template<size_t N>
  class TableNode : public iTableNode
  {
  public:
    /**
     * \brief Конструктор узла с потомками
     * \param tables Список потомков (узлы или таблицы)
     * \param start_address Смещение адресов потомков
     */
    template<typename... TTables>
    constexpr TableNode(
      size_t start_address, TTables&... tables, ...
    )
    : iTableNode{_buffer, start_address},
      _buffer{&tables...}
    {
    }

  private:
    //! Хранилище указателей на таблицы в узле
    etl::array<iTable*, N> _buffer;
  };


  template<typename... TTables>
  TableNode(size_t, TTables...) -> TableNode<sizeof...(TTables)>;


  /**
   * \brief Узел дерева меню с узлами-потомками
   * \tparam TNodes Список узлов-потомков
   */
  template<size_t N>
  class TreeNode : public iTableNode
  {
  public:
    /**
     * \brief Конструктор узла с потомками
     * \param nodes Список узлов-потомков
     * \param start_address Смещение адресов потомков
     */
    template<typename... TNodes>
    constexpr TreeNode(size_t start_address, TNodes&... nodes, ...)
    : iTableNode{_buffer, start_address},
      _buffer{&nodes...}
    {
    }

  private:
    //! Хранилище указателей на узлы-потомки
    etl::array<const iTableNode*, N> _buffer;
  };


  template<typename... TNodes>
  TreeNode(size_t, TNodes...) -> TreeNode<sizeof...(TNodes)>;


  //! Структура данных меню в дереве
  struct TableTreeData
  {
    //! Ссылка на таблицу меню в дереве
    iTable* table;
    //! Первый адрес меню в дереве
    size_t start_address; 
    //! Терминирующий адрес меню в дереве (следующий за последним)
    size_t end_address; 
  };


  /**
   * \brief Интерфейс дерева меню
   */
  class iTableTree
  {
  public:
    //! Тип адреса индекса переменных
    using size_type = iTable::size_type;

    //! Тип данных списка узлов для инициализации дерева
    using init_span = etl::span<const iTableNode*>;

    //! Тип данных вектора структуры меню в дереве
    using itable_data = etl::ivector<TableTreeData>;
    //! Тип данных хранилища структуры меню в дереве
    template<size_t MAX_SIZE>
    using table_data = etl::vector<TableTreeData, MAX_SIZE>;

    //! Итератор структуры меню в дереве
    using iterator = itable_data::iterator;
    //! Const итератор структуры меню в дереве
    using const_iterator = itable_data::const_iterator;
    //! Ссылка на структуру меню в дереве
    using reference = itable_data::reference;
    //! Const ссылка на структуру меню в дереве
    using const_reference = itable_data::const_reference;

    /**
     * \brief Ссылка на элемент переменной в дереве таблиц параметров
     * \todo Добавить операторы сравнения и присвоения
     * \todo Вынести element и связанные с ним методы за iTableTree,
     * добавить iTableTree как ссылку в структуру
     */
    struct element
    {
      //! Итератор текущего элемента в дереве
      const_iterator table_it;

      //! Данные переменной текущего элемента
      TableVariable table_var;

      //! Адрес текущего элемента в дереве
      size_t address;

      //! Смещение в байтах относительно начального адреса переменной
      size_t offset;

      //! Флаг смены таблицы меню при обходе
      bool is_table_switched{true};

      //! Флаг смены переменной меню при обходе
      bool is_variable_switched{true};

      /**
       * \brief Копирует данные по указателю с добавлением смещения элемента
       * \param[out] to
       * \param[in] from
       * \param[in] size Кол-во копируемых байт
       * \details
       * Имеет защиту от выхода за размер переменной при копировании
       * \todo Добавить нормальное описание здесь и ниже
       */
      void copy_to_offset(void* to, const void* from, size_t size) const;

      /**
       * \brief Копирует данные со смещением элемента по указателю
       * \param[out] to
       * \param[in] from
       * \param[in] size Кол-во копируемых байт
       * \details
       * Имеет защиту от выхода за размер переменной при копировании
       */
      void copy_from_offset(void* to, const void* from, size_t size) const;
    };

    //! Шаг обхода для адресации, байт
    const size_t step;

  private:
    //! Ссылка на таблицу соответствия адрес/меню
    itable_data& _tables;

    //! Ссылка на корневой узел дерева
    const iTableNode* _root;

    //! Маска флагов для включения в дерево
    const flag::TYPE include_if;

  protected:
    /**
     * \brief Конструктор дерева меню
     * \param tree_storage Ссылка на хранилище структуры дерева
     * \param root Ссылка на корень дерева, по которому собирается структура дерева
     * \param step Шаг обхода дерева в байтах (0 - обход по индексу)
    */
    iTableTree(
      itable_data& tree_storage,
      const iTableNode& root,
      const size_t step,
      flag::TYPE include_if={}
    );

  public:
    //! Пустой узел дерева (указывает на \ref menu::iTable::dummy_table)
    static const iTableNode& dummy_node;

    /**
     * \brief Возвращает итератор на начало таблицы адрес/меню
    */
    iterator begin() { return _tables.begin(); }

    /**
     * \brief Возвращает итератор на конец таблицы адрес/меню
    */
    iterator end() { return _tables.end(); }

    /**
     * \brief Возвращает const итератор на начало таблицы адрес/меню
    */
    const_iterator begin() const { return _tables.begin(); }

    /**
     * \brief Возвращает const итератор на конец таблицы адрес/меню
    */
    const_iterator end() const { return _tables.end(); }

    /**
     * \brief Возвращает ссылку на первый элемент таблицы адрес/меню
     */
    reference front() { return _tables.front(); }

    /**
     * \brief Возвращает const ссылку на первый элемент таблицы адрес/меню
     */
    reference front() const { return _tables.front(); }

    /**
     * \brief Возвращает ссылку на последний элемент таблицы адрес/меню
     */
    reference back() { return _tables.back(); }

    /**
     * \brief Возвращает const итератор на последний элемент таблицы адрес/меню
     */
    const_reference back() const { return _tables.back(); }

    /**
     * \brief Ищет данные о таблице в дереве меню
     * \param table Искомая таблица
     * \return Итератор на данные таблицы
     * \return \ref _tables.end(), если таблица не входит в дерево
     */
    iterator find(iTable& table) const;

    /**
     * \brief Возвращает элемент дерева по таблице меню 
     * \param table Искомая таблица меню
     * \return элемент первой включенной в дерево переменной таблицы меню, если 
     * таблица входит в дерево
     * \return элемент пустышки \ref dummy_table, если таблица и/или все
     * её переменные не входят в дерево
     */
    element getElementByTable(menu::iTable& table) const;

    /**
     * \brief Возвращает элемент переменной в дереве таблицы меню по положению
     * переменной в меню
     * \details
     * Расчет идет с учетом шага \ref step и условия \ref include_if
     * \return элемент переменной, если переменная с таблицей входит в дерево
     * \return элемент пустышки \ref dummy_table, если переменная и/или таблица
     * не входят в дерево
     */
    element getElementByPosition(iTable& table, size_t position) const;

    /**
     * \brief Возвращает элемент переменной в дереве таблицы меню по переменной
     * в меню
     * \details
     * Расчет идет с учетом шага \ref step и условия \ref include_if
     * \return элемент переменной, если переменная с таблицей входит в дерево
     * \return элемент пустышки \ref dummy_table, если переменная и/или таблица
     * не входят в дерево
     */
    element getElementByVariable(iTable& table, const ivariable& variable)
    const;

    /**
     * \brief Возвращает элемент переменной в дереве таблицы меню по адресу
     * переменной в меню
     * \details
     * Расчет идет с учетом шага \ref step и условия \ref include_if
     * \return элемент переменной, если переменная с таблицей входит в дерево
     * \return элемент пустышки \ref dummy_table, если переменная и/или таблица
     * не входят в дерево
     */
    element getElementByAddress(size_t address) const;

    /**
     * \brief Возвращает элемент первой переменной в дереве
     */
    element getFirstElement() const;

    /**
     * \brief Возвращает терминирующий элемент дерева меню
     */
    element getEndElement() const;

    /**
     * \brief Выбирает следующую переменную в дереве меню
     */
    void advanceElementByVariable(element& current) const;

    /**
     * \brief Выбирает следующий адрес в дереве меню
     */
    void advanceElementByAddress(element& current) const;

    /**
     * \brief Выбирает следующую таблицу в дереве меню
     * \return Элемент первой включенной в дерево для следующей таблицы в меню
     * \return Элемент-пустышка с адресом \ref size(), если таблицы для обхода
     * закончились
     */
    void advanceElementByTable(element& current) const;

    /**
     * \brief Возвращает первую включенную в дерево переменную таблицы меню
     * \details Ищет с учетом \ref include_if
     * \attention Не проверяет наличие таблицы меню в дереве, только проверяет
     * условия для переменных
     * \return Первая переменная таблицы, выполняющая условие \ref include_if
     * \return \ref iTable::getDummyVariable(), если все переменные не входят
     * в дерево
     */
    TableVariable getFirstVariable(iTable& table) const;

    /**
     * \brief Возвращает размер переменной в дереве меню
     * \details
     * Размер кратен \ref _step.
     * 
     * Для step=0 размер соответствует 1, т.к. идет счет по индексу переменной.
    */
    size_t getVariableSize(const ivariable& variable) const;

    /**
     * \brief Возвращает первый адрес дерева
     */
    size_t start_address() const;

    /**
     * \brief Возвращает терминирующий адрес дерева
     */
    size_t end_address() const;

    /**
     * \brief Перерасчитывает дерево меню 
     * \todo Добавить сортировку дерева по начальным адресам для правильного
     * определения конечного адреса меню
     * (чтобы не было в первой строке дерева смещения 200, а в последней - 100)
     */
    void recalculateTree();


    /**
     * \brief Перерасчитывает дерево меню для нового корневого узла
     * \param new_root Новый корневой узел
     * \attention Узел должен быть глобальной переменной или доступен всегда
     * \todo Добавить сортировку дерева по начальным адресам для правильного
     * определения конечного адреса меню
     * (чтобы не было в первой строке дерева смещения 200, а в последней - 100)
     */
    void recalculateTree(const menu::iTableNode& new_root);

  private:
    /**
     * \brief Инициализация дерева
     * \details
     * Проходит по всем узлам дерева, рассчитывает адреса меню и
     * добавляет их в таблицу адрес/меню.
     * 
     * Пропускает меню, если каждая переменная в нем не удовлетворяет условию.  
     * Не добавляет пустые таблицы.
     * \ref include_if
     * \return Терминирующий адрес для дерева
     */
    size_t init(const iTableNode& root, const size_t start_address={});

    /**
     * \brief Рассчитывает размер меню в дереве с учетом условий
    */
    size_t calculate_size(iTable& table) const;

    /**
     * \brief Проверяет включена ли переменная в дерево меню
     * \note Не проверяет входит ли меню в дерево
     * \return true, если переменная включена
    */
    bool is_variable_included(const ivariable& variable) const;

    /**
     * \brief Возвращает элемент-пустышку
     * \param table_data Интератор текущего положения таблицы в дереве
     * \param address Адрес переменной-пустышки
     */
    element get_dummy_element(
      const_iterator table_data,
      const size_t address=etl::numeric_limits<size_t>::max()
    ) const;
  };


  /**
   * \brief Сравнение адресов двух строк таблиы дерева
   */
  struct less_table_data_address : public 
  etl::binary_function<TableTreeData, TableTreeData, bool>
  {
    constexpr bool operator()(
      const TableTreeData& lhs, const TableTreeData& rhs
    ) const
    {
      return lhs.start_address < rhs.start_address;
    }
  };


  /**
   * \brief Сравнение двух элементов дерева
   */
  bool operator==(const iTableTree::element&, const iTableTree::element&);


  /**
   * \brief Сравнение на несовпадение двух элементов дерева
   */
  bool operator!=(const iTableTree::element&, const iTableTree::element&);


  /**
   * \brief Дерево меню с выделенным хранилищем
   * \tparam SIZE Максимальное количество меню в дереве
  */
  template<size_t SIZE>
  class TableTree : public iTableTree
  {
  public:
    /**
     * \brief Конструктор дерева меню
     * \param root Корень дерева
     * \param step Шаг обхода переменных меню в байтах. 0 - обход по индексу
     * \param include_if Требование к флагам для включения переменной в меню
     * (маска )
    */
    TableTree(
      const iTableNode& root,
      size_t step,
      flag::TYPE include_if={}
    )
    : iTableTree{_buffer, root, step, include_if}
    {
      recalculateTree();
    }

  private:
    table_data<SIZE> _buffer;
  };
}

#endif /* MENU_H */
