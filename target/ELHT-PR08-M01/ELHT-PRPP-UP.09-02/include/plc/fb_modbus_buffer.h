/**
 * \file plc/plc_modbus_buffer.h
 * \brief Функции для работы с буфером Modbus библиотеки PLC
 * \ingroup plc_com_modbus
 */

#pragma once 

#include "plc/base.h"
#include "modbus/modbus_menu.h"


namespace plc
{
  /**
   * \addtogroup plc_com_modbus
   * \{
   */

  //! Тип данных для таблицы регистров Modbus
  using modbus_register_t = uint16_t;

  //! Описание типа переменной регистра Modbus
  inline const menu::var<modbus_register_t> modbus_variable;
  //! Описание таблицы параметров с регистрами Modbus
  inline const menu::TableInfo modbus_buffer_info{modbus_variable};

  using ModbusBufferInfo = decltype(modbus_buffer_info);


  /**
   * \brief Базовый интерфейс пользовательского буфера Modbus
   */
  class iModbusBufferBase : protected component_link
  {
  public:
    //! Таблица регистров
    menu::iTable& register_table;

    //! Конструктор пользовательского буфера Modbus
    iModbusBufferBase(menu::iTable& register_table)
    : register_table{register_table},
      _locked{false}
    {
    }

    //! Возвращает кол-во регистров в буфере
    size_t size() const { return register_table.size(); }

    /**
     * \brief Блокирует таблицу регистров для текущего POU
     * \details
     * \note При вызове функций чтения/записи буфера Modbus таблица
     * блокируется автоматически
     * \retval true Таблица заблокирована
     * \retval false Ошибка блокировки таблицы
     */
    bool lock();

    /**
     * \brief Сбрасывает блокировку таблицы регистров
     * \details
     * При выходе из POU таблицы регистров сбрасываются автоматически.
     * 
     * Функция может использоваться вручную после операций чтения/записи, если
     * POU занимает много времени на выполнение
     * \retval true Таблица разблокирована
     * \retval false Ошибка разблокировки таблицы
     */
    bool unlock();

  protected:
    inline static component_list<iModbusBufferBase> _list;
    bool _locked;

    friend component_list<iModbusBufferBase>;
    friend class ComponentListHandler;
  };


  /**
   * \brief Интерфейс буфера пользовательских регистров Modbus
   * \details
   * Содержит список функций, которые можно использовать 
   * в пользовательском буфере регистров Modbus.
   * 
   * \note Пользовательский буфер создается через класс \ref ModbusBuffer<N>
   */
  class iModbusBuffer : public iModbusBufferBase
  {
  public:
    /**
     * \brief Шаблон функции считывания переменной из таблицы регистров
     * \tparam T Тип считываемой переменной
     * \param index Номер регистра в таблице регистров
     * 
     * \details
     * \note Если тип считываемой переменной занимает больше одного регистра
     * Modbus (например, int - 32 бита, регистр - 16 бит), то функция 
     * считает и следующий за index регистр
     * 
     * \return Значение переменной
     */
    template<typename T>
    T read(uint16_t index);

    /**
     * \brief Шаблон функции записи переменной в таблицу регистров
     * \tparam T Тип записываемой переменной
     * \param index Номер регистра в таблице регистров
     * \param value Записываемое значение переменной
     * 
     * \details
     * \note Если тип записываемой переменной занимает больше одного регистра
     * Modbus (например, int - 32 бита, регистр - 16 бит), то функция 
     * перезапишет и следующий за index регистр
     * 
     * \retval true если переменная успешно записана в таблицу регистров
     * \retval false если произошла ошибка записи
     */
    template<typename T>
    bool write(uint16_t index, T value);

    /**
     * \brief Считывает из регистра переменную типа bool
     * \param index Адрес регистра
     * \details
     * Преобразует считанное значение регистра в bool (логическая переменная,
     * 0 = false, иначе true).
     */
    bool readBool(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа char
     * \param index Адрес регистра
     * \details
     * Преобразует младшие 8 бит регистра в char (целое 8-битное число
     * без знака). Игнорирует старшие 8 бит регистра.
     */
    char readChar(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа short
     * \param index Адрес регистра
     * \details 
     * Преобразует значение регистра в short (целое 16-битное число со знаком)
     */
    short readShort(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа int
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1" в int
     * (целое 32-битное число со знаком)
     */
    int readInt(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа long
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1" в long
     * (целое 32-битное число со знаком)
     */
    long readLong(uint16_t index);

    /**
     * \brief Считывает из четырех регистров переменную типа long long
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам от "index" до "index + 3"
     * в long long (целое 64-битное число со знаком)
     */
    long long readLongLong(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа float
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1" в float
     * (32-битное число с плавающей точкой одинарной точности)
     */
    float readFloat(uint16_t index);

    /**
     * \brief Считывает из четырех регистров переменную типа double
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам от "index" до "index + 3"
     * в double (64-битное число с плавающей точкой двойной точности)
     */
    double readDouble(uint16_t index);

    /**
     * \brief Считывает из первого регистра переменную типа signed char
     * \param index Адрес регистра
     * \details
     * Преобразует младшие 8 бит регистра в signed char (целое 8-битное число 
     * со знаком). Игнорирует старшие 8 бит регистра
     */
    signed char readSignedChar(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа signed short
     * \param index Адрес регистра
     * \details 
     * Преобразует значение регистра в signed short (целое 16-битное число 
     * со знаком)
     */
    signed short readSignedShort(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа signed int
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1"
     * в signed int (целое 32-битное число со знаком)
     */
    signed int readSignedInt(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа signed long
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1"
     * в signed long (целое 32-битное число со знаком)
     */
    signed long readSignedLong(uint16_t index);

    /**
     * \brief Считывает из четырех региcтров переменную типа signed long long
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам от "index" до "index + 3"
     * в signed long long (целое 64-битное число со знаком)
     */
    signed long long readSignedLongLong(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа unsigned char
     * \param index Адрес регистра
     * \details
     * Преобразует младшие 8 бит регистра в unsigned char (целое 8-битное число
     * без знака). Игнорирует старшие 8 бит регистра
     */
    unsigned char readUnsignedChar(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа unsigned short
     * \param index Адрес регистра
     * \details 
     * Преобразует значение регистра в unsigned short (целое 16-битное число
     * без знака)
     */
    unsigned short readUnsignedShort(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа unsigned int
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1"
     * в usigned int (целое 32-битное число без знака)
     */
    unsigned int readUnsignedInt(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа unsigned long
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1" 
     * в unsigned long (целое 32-битное число без знака)
     */
    unsigned long readUnsignedLong(uint16_t index);

    /**
     * \brief Считывает из четырех региcтров переменную типа unsigned long long
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам от "index" до "index + 3"
     * в unsigned long long (целое 64-битное число без знака)
     */
    unsigned long long readUnsignedLongLong(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа int8_t
     * \param index Адрес регистра
     * \details
     * Преобразует младшие 8 бит регистра в int8_t (целое 8-битное число 
     * со знаком). Игнорирует старшие 8 бит регистра
     */
    int8_t readInt8(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа int16_t
     * \param index Адрес регистра
     * \details
     * Преобразует значение регистра в int16_t (целое 16-битное число со знаком)
     */
    int16_t readInt16(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа int32_t
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1"
     * в int32_t (целое 32-битное число со знаком)
     */
    int32_t readInt32(uint16_t index);

    /**
     * \brief Считывает из четырех регистров переменную типа int64_t
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам от "index" до "index + 3"
     * в int64_t (целое 64-битное число со знаком)
     */
    int64_t readInt64(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа uint8_t
     * \param index Адрес регистра
     * \details
     * Преобразует младшие 8 бит регистра в uint8_t (целое 8-битное число
     * без знака). Игнорирует старшие 8 бит регистра
     */
    uint8_t readUint8(uint16_t index);

    /**
     * \brief Считывает из регистра переменную типа uint16_t
     * \param index Адрес регистра
     * \details
     * Преобразует значение регистра в uint16_t (целое 16-битное число
     * без знака)
     */
    uint16_t readUint16(uint16_t index);

    /**
     * \brief Считывает из двух регистров переменную типа uint32_t
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам "index" и "index + 1"
     * в uint32_t (целое 32-битное число без знака)
     */
    uint32_t readUint32(uint16_t index);

    /**
     * \brief Считывает из четырех регистров переменную типа uint64_t
     * \param index Адрес первого регистра
     * \details
     * Преобразует значения регистров по адресам от "index" до "index + 3"
     * в uint64_t (целое 64-битное число без знака)
     */
    uint64_t readUint64(uint16_t index);
    
    /**
     * \brief Записывает в регистр переменную типа bool
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа bool (логическая переменная,
     * 0 = false, иначе true) в регистр Modbus
     */
    bool writeBool(uint16_t index, bool value);

    /**
     * \brief Записывает в регистр переменную типа char
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа char (целое 8-битное
     * число без знака)
     */
    bool writeChar(uint16_t index, char value);

    /**
     * \brief Записывает в регистр переменную типа short
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа short (целое
     * 16-битное число со знаком)
     */
    bool writeShort(uint16_t index, short value);

    /**
     * \brief Записывает в два регистра переменную типа int
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа int (целое 32-битное число
     * со знаком) в регистры с адресами от "index" до "index + 1"
     */
    bool writeInt(uint16_t index, int value);

    /**
     * \brief Записывает в два регистра переменную типа long
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа long (целое 32-битное число
     * со знаком) в регистры с адресами от "index" до "index + 1"
     */
    bool writeLong(uint16_t index, long value);

    /**
     * \brief Записывает в четыре регистра переменную типа long long
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа long long (целое 64-битное 
     * число со знаком) в регистры с адресами от "index" до "index + 3"
     */
    bool writeLongLong(uint16_t index, long long value);

    /**
     * \brief Записывает в два регистра переменную типа float
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа float (32-битное число
     * с плавающей точкой одинарной точности) в регистры с адресами
     * от "index" до "index + 1"
    */
    bool writeFloat(uint16_t index, float value);

    /**
     * \brief Записывает в четыре регистра переменную типа double
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа double (64-битное число
     * с плавающей точкой двойной точности) в регистры с адресами
     * от "index" до "index + 3"
    */
    bool writeDouble(uint16_t index, double value);

    /**
     * \brief Записывает в регистр переменную типа signed char
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа signed char (целое
     * 8-битное число со знаком)
     */
    bool writeSignedChar(uint16_t index, signed char value);

    /**
     * \brief Записывает в регистр переменную типа signed short
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа signed short (целое
     * 16-битное число со знаком)
     */
    bool writeSignedShort(uint16_t index, signed short value);

    /**
     * \brief Записывает в два регистра переменную типа signed int
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа signed int (целое 32-битное
     * число со знаком) в регистры с адресами от "index" до "index + 1"
     */
    bool writeSignedInt(uint16_t index, signed int value);

    /**
     * \brief Записывает в два регистра переменную типа signed long
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа signed long (целое 32-битное
     * число со знаком) в регистры с адресами от "index" до "index + 1"
     */
    bool writeSignedLong(uint16_t index, signed long value);

    /**
     * \brief Записывает в четыре регистра переменную типа signed long long
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа signed long long (целое
     * 64-битное число со знаком) в регистры с адресами от "index"
     * до "index + 3"
     */
    bool writeSignedLongLong(uint16_t index, signed long long value);

    /**
     * \brief Записывает в регистр переменную типа unsigned char
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа unsigned char (целое 
     * 8-битное число без знака)
     */
    bool writeUnsignedChar(uint16_t index, unsigned char value);

    /**
     * \brief Записывает в регистр переменную типа unsigned short
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа unsigned short (целое 
     * 16-битное число без знака)
     */
    bool writeUnsignedShort(uint16_t index, unsigned short value);

    /**
     * \brief Записывает в два регистра переменную типа unsigned int
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа unsigned int (целое 32-битное 
     * число без знака) в регистры с адресами от "index" до "index + 1"
     */
    bool writeUnsignedInt(uint16_t index, unsigned int value);

    /**
     * \brief Записывает в два регистра переменную типа unsigned long
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа unsigned long (целое 32-битное
     * число без знака) в регистры с адресами от "index" до "index + 1"
     */
    bool writeUnsignedLong(uint16_t index, unsigned long value);

    /**
     * \brief Записывает в четыре регистра переменную типа unsigned long long
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа unsigned long long (целое 
     * 64-битное число без знака) в регистры с адресами от "index" 
     * до "index + 3"
     */
    bool writeUnsignedLongLong(uint16_t index, unsigned long long value);

    /**
     * \brief Записывает в регистр переменную типа int8_t
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа int8_t (целое 
     * 8-битное число со знаком)
     */
    bool writeInt8(uint16_t index, int8_t value);

    /**
     * \brief Записывает в регистр переменную типа int16_t
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа int16_t (целое 
     * 16-битное число со знаком)
     */
    bool writeInt16(uint16_t index, int16_t value);

    /**
     * \brief Записывает в два регистра переменную типа int32_t
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа int32_t (целое 32-битное число 
     * со знаком) в регистры с адресами от "index" до "index + 1"
     */
    bool writeInt32(uint16_t index, int32_t value);

    /**
     * \brief Записывает в четыре регистра переменную типа int64_t
     * \param index Адрес первого регистра
     * \details
     * Преобразует и записывает переменную типа int64_t (целое 64-битное число
     * со знаком) в регистры с адресами от "index" до "index + 3"
     */
    bool writeInt64(uint16_t index, int64_t value);

    /**
     * \brief Записывает в регистр переменную типа uint8_t
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа uint8_t (целое 
     * 8-битное число без знака)
     */
    bool writeUint8(uint16_t index, uint8_t value);

    /**
     * \brief Записывает в регистр переменную типа uint16_t
     * \param index Адрес регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает в регистр переменную типа uint16_t (целое 
     * 16-битное число без знака)
     */
    bool writeUint16(uint16_t index, uint16_t value);

    /**
     * \brief Записывает в два регистра переменную типа uint32_t
     * \param index Адрес первого регистра
     * \param value Значение переменной
     * \details
     * Преобразует и записывает переменную типа uint32_t (целое 32-битное число 
     * без знака) в регистры с адресами от "index" до "index + 1"
     */
    bool writeUint32(uint16_t index, uint32_t value);

    /**
     * \brief Записывает в четыре регистра переменную типа uint64_t
     * \param index Адрес первого регистра
     * \details
     * Преобразует и записывает переменную типа uint64_t (целое 64-битное число
     * без знака) в регистры с адресами от "index" до "index + 3"
     */
    bool writeUint64(uint16_t index, uint64_t value);

  protected:
    iModbusBuffer(menu::iTable& register_table)
    : iModbusBufferBase{register_table}
    {
    }
  };


  /**
   * \brief Буфер регистров Modbus
   * \tparam N Кол-во регистров в буфере
   * \details
   * Пример создания буфера с именем "buffer" на 100 регистров Modbus:
   * \code{.cpp}
   * ModbusBuffer<100> buffer;
   * \endcode
   */
  template<uint16_t N>
  class ModbusBuffer : public iModbusBuffer
  {
  public:
    ModbusBuffer()
    : iModbusBuffer{_register_table_storage}
    {
    }

  private:
    elh::mutex _mutex{};
    menu::Table<N, ModbusBufferInfo> _register_table_storage{
      modbus_buffer_info, &_mutex
    };
  };


  /**
   * \brief Интерфейс буфера работы с Coils и Discrete Inputs
   * \details
   * Содержит список функций, которые можно использовать 
   * в пользовательском буфере Coils/Discrete INputs.
   * 
   * \note Пользовательский буфер создается через класс \ref ModbusCoilBuffer<N>
   */
  class iModbusCoilBuffer : public iModbusBufferBase
  {
  public:
    /**
     * \brief Устанавливает состояние Coil/Discrete Input
     * \param index Адрес Coil/Discrete Input
     * \param state Устанавливаемое состояние
     * \retval true Состояния записаны
     * \retval false Ошибка записи
     */
    bool writeCoil(uint16_t index, bool state);

    /**
     * \brief Считывает состояние Coil/Discrete Input
     * \param index Адрес Coil/Discrete Input
     */
    bool readCoil(uint16_t index);

    /**
     * \brief Записывает битовую маску до 32 Coils/Discrete Inputs
     * \param start_index Адрес первого Coil/Discrete Input
     * \param quantity Кол-во записываемых Coils/Discrete Inputs (максимум 32)
     * \param bitmask Записываемая битовая маска
     */
    bool writeCoilsBitmask(
      uint16_t start_index, uint8_t quantity, uint32_t bitmask
    );

    /**
     * \brief Считывает битовую маску до 32 Coils/Discrete Inputs
     * \param start_index Адрес первого Coil/Discrete Input
     * \param quantity Кол-во считываемых Coils/Discrete Inputs (максимум 32)
     */
    uint32_t readCoilsBitmask(uint16_t start_index, uint8_t quantity);

  protected:
    iModbusCoilBuffer(menu::iTable& register_table)
    : iModbusBufferBase{register_table}
    {
    }
  };


  /**
   * \brief Буфер Coils и Discrete Inputs
   * \tparam N Кол-во Coils/Discrete Inputs в буфере
   * \details
   * Пример создания буфера с именем coil_buffer на 100 Coils:
   * \code{.cpp}
   * ModbusCoilBuffer<100> coil_buffer;
   * \endcode
   */
  template<uint16_t N>
  class ModbusCoilBuffer : public iModbusCoilBuffer
  {
  public:
    /**
     * \brief Конструктор буфера Coils/Discrete Inputs
     */
    ModbusCoilBuffer()
    : iModbusCoilBuffer{_register_table_storage}
    {
    }

  private:
    elh::mutex _mutex{};
    modbus::ModbusCoilTable<N> _register_table_storage{
      modbus::modbus_coil_menu_info, &_mutex
    };
  };


  /**
   * \}
   * \noop plc_com_modbus
   */
}
