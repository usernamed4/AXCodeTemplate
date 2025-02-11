/**
 * \file dio.h
 * \brief Функции дискретных входов и выходов
 * \ingroup plc_dio
 */

#pragma once

#include "plc/fb_device.h"


namespace plc
{
  /**
   * \addtogroup plc_dio
   * \{
   */

  /**
   * \brief Дискретный вход
   */
  class DiscreteInput : protected etl::forward_link<0>
  {
  public:
    /**
     * \brief Конструктор дискретного входа по ссылке
     * \param device Устройство с дискретным входом
     * \param channel Номер дискретного входа
     */
    DiscreteInput(device::iDevice& device, uint16_t channel);

    /**
     * \brief Конструктор дискретного входа по указателю
     * \param device Устройство с дискретным входом
     * \param channel Номер дискретного входа
     */
    DiscreteInput(device::iDevice* device, uint16_t channel);

    //! Нулевой указатель на устройство запрещен
    DiscreteInput(nullptr_t device, uint16_t) =delete;

    //! Оператор преобразования значения входа в bool
    operator bool() const { return value(); }
   
    //! Оператор равенства
    bool operator==(const bool other) const;

    //! Оператор неравенства
    bool operator!=(const bool other) const;

    //! Возвращает последнее считанное состояние дискретного входа
    bool value() const;
    //! Возвращает передний фронт дискретного входа
    bool front();
    //! Возвращает задний фронт дискретного входа
    bool back();
    //! Возвращает указатель на устройство со входом
    device::iDevice* device() const;
    //! Возвращает номер входа в устройстве
    uint16_t channel() const;
    //! Возвращает время, прошедшее с обновления состояния входа
    int32_t time_since_update() const;

    //! Возвращает состояние опроса дискретного входа (вкл/выкл)
    bool isEnabled() const;

    /**
     * \brief Включает/выключает опрос дискретного входа
     * \param state Состояние опроса дискретного входа (вкл/выкл)
     */
    void enable(bool state=true);

    //! Выключает опрос дискретного входа
    void disable();

    //! Считывает состояние дискретного входа из привязанного устройства 
    void scan();

  private:
    enum MASK : uint8_t
    {
      MASK_NONE  = 0x00,
      MASK_VALUE = 0x01,
      MASK_LAST  = 0x02,
      MASK_FRONT = 0x04,
      MASK_BACK  = 0x08,
      MASK_RESET_FRONT = 0x10,
      MASK_RESET_BACK  = 0x20,
      MASK_DISABLED    = 0x40
    };

    using list_t = etl::intrusive_list<DiscreteInput, etl::forward_link<0>>;

    inline static list_t _list;
    uint32_t _last_update_time;
    device::iDevice& _device;
    uint16_t _channel;
    uint8_t _mask;

    friend list_t;
    friend void DIO_Scan();
  };


  /**
   * \brief Массив дискретных входов
   * \tparam N Размер массива
   */
  template<size_t N>
  class DiscreteInputArray : public ChannelArray<DiscreteInput, N>
  {
  public:
    /**
     * \brief Конструктор массива дискретных входов
     * \param device Устройство с дискретными входами
     * \param start_channel Номер первого канала дискретного входа в массиве
     * \details
     * Создаст массив дискретных входов с каналами 
     * от start_channel до start_channel + N.
     * 
     * Пример создания массива на 4 элемента для устройства "device",
     * начиная со 2-го канала:
     * \code{.cpp}
     * DiscreteInputArray<4> inputs(device, 2);
     * \endcode
     */
    DiscreteInputArray(device::iDevice& device, uint16_t start_channel=0)
    : ChannelArray<DiscreteInput, N>{device, start_channel}
    {
    }

    /**
     * \brief Конструктор массива дискретных входов по указателю на устройство
     * \param device Указатель на устройство с дискретными входами
     * \param start_channel Номер первого канала дискретного входа в массиве
     */
    DiscreteInputArray(device::iDevice* device, uint16_t start_channel=0)
    : DiscreteInputArray{*device, start_channel}
    {
    }

    //! Нулевой указатель на устройство запрещен
    DiscreteInputArray(nullptr_t, uint16_t) =delete;
  };


  /**
   * \brief Дискретный выход
   */
  class DiscreteOutput : protected etl::forward_link<0>
  {
  public:
    /**
     * \brief Конструктор дискретного выхода
     * \param device Устройство с дискретным выходом
     * \param channel Номер дискретного выхода
     */
    DiscreteOutput(device::iDevice& device, uint16_t channel);

    /**
     * \brief Конструктор дискретного выхода
     * \param device Устройство с дискретным выходом
     * \param channel Номер дискретного выхода
     */
    DiscreteOutput(device::iDevice* device, uint16_t channel);

    //! Нулевой указатель на устройство запрещен
    DiscreteOutput(nullptr_t device, uint16_t channel) =delete;

    //! Оператор преобразования состояния выхода в bool
    operator bool() const { return value(); }

    //! Преобразование выхода в другие типы запрещено
    template<typename T>
    operator T() const=delete; 

    /**
     * \brief Оператор присвоения состояния дискретного выхода
     * \param value Новое состояние дискретного выхода
     */
    DiscreteOutput& operator=(const bool value);

    /**
     * \brief Оператор присвоения состояния дискретного выхода из переменных
     * прочих типов (кроме указателей)
     * \tparam T Тип преобразуемой переменной
     * \param value Новое состояние дискретного выхода
     */
    template<typename T>
    DiscreteOutput& operator=(const T& value)
    {
      return operator=(static_cast<bool>(value));
    }

    //! Оператор равенства
    bool operator==(const bool other) const;

    //! Оператор неравенства
    bool operator!=(const bool other) const; 

    /**
     * \brief Задает новое состояние дискретного выхода
     * \param new_value Новое состояние
     */
    void set(const bool new_value);

    //! Возвращает заданное состояние выхода
    bool value() const;

    //! Возвращает указатель на устройство с выходом
    device::iDevice* device() const;
    //! Возвращает номер входа в устройстве
    uint16_t channel() const;
    //! Возвращает время, прошедшее с обновления состояния входа
    int32_t time_since_update() const;

    //! Возвращает состояние опроса дискретного выхода (вкл/выкл)
    bool isEnabled() const;

    /**
     * \brief Включает/выключает опрос дискретного выхода
     * \param state Состояние опроса дискретного выхода (вкл/выкл)
     */
    void enable(bool state=true);

    //! Выключает опрос дискретного выхода
    void disable();

    //! Записывает в привязанное устройство заданное состояние выхода
    void update();

  private:
    using list_t = etl::intrusive_list<DiscreteOutput, etl::forward_link<0>>;

    inline static list_t _list;
    uint32_t _last_update_time;
    device::iDevice& _device;
    uint16_t _channel;
    bool _value;
    bool _enabled;

    friend list_t;
    friend void DIO_Update();
  };


  /**
   * \brief Массив дискретных выходов
   * \tparam N Размер массива
   * \tparam DEVICE_T Класс устройства с дискретными выходами
   */
  template<size_t N>
  class DiscreteOutputArray : public ChannelArray<DiscreteOutput, N>
  {
  public:
    /**
     * \brief Конструктор массива дискретных выходов
     * \param device Устройство с дискретными выходами
     * \param start_channel Номер первого канала дискретного выхода в массиве
     */
    DiscreteOutputArray(device::iDevice& device, uint16_t start_channel=0)
    : ChannelArray<DiscreteOutput, N>{device, start_channel}
    {
    }

    /**
     * \brief Конструктор массива дискретных выходов по указателю на устройство
     * \param device Указатель на устройство с дискретными выходами
     * \param start_channel Номер первого канала дискретного выхода в массиве
     */
    DiscreteOutputArray(device::iDevice* device, uint16_t start_channel=0)
    : DiscreteOutputArray{*device, start_channel}
    {
    }

    //! Нулевой указатель на устройство запрещен
    DiscreteOutputArray(nullptr_t, uint16_t) =delete;
  };


  /**
   * \brief Функция считывания состояний дискретных входов из привязанных
   * устройств
   * \details
   * Выполняется автоматически перед циклом вызова POU
   */
  void DIO_Scan();

  /**
   * \brief Функция записи состояний дискретных выходов в привязанные
   * устройства
   * \details
   * Выполняется автоматически после цикла вызова POU
   */ 
  void DIO_Update();

  /**
   * \}
   * \noop plc_dio
   */
}
