/**
 * \file plc/aio.h
 * \brief Функции аналоговых входов и выходов
 */

#pragma once

#include "plc/fb_device.h"


namespace plc
{
  /**
   * \addtogroup plc_aio
   * \{
   */

  /**
   * \brief Сервисный класс для общих функций аналоговых входов и выходов
   * \details
   * - Для работы с аналоговыми входами используется \ref AnalogInput
   * - Для работы с аналоговыми выходами используется \ref AnalogOutput
   */
  class __AnalogBase
  {
  public: 
    //! Считывает аналоговое значение
    float value() const;

    //! Преобразует аналоговое значение в указанный тип данных
    template<typename T>
    operator T() const
    {
      return static_cast<T>(value());
    }

    //! Возвращает значение выхода при преобразовании класса в тип данных float
    operator float() const
    {
      return value();
    }

    //! Возвращает указатель на устройство со входом
    device::iDevice* device() const;
    //! Возвращает номер входа в устройстве
    uint16_t channel() const;
    //! Возвращает состояние опроса входа/выхода (вкл/выкл)
    bool isEnabled() const;

    /**
     * \brief Включает/выключает опрос входа/выхода
     * \param state Состояние опроса входа/выхода (вкл/выкл)
     */
    void enable(bool state=true);

    //! Выключает опрос входа/выхода
    void disable();

  protected:
    __AnalogBase(device::iDevice& device, uint16_t channel, float value);

    void __set(float new_value);

    float _value;
    device::iDevice& _device;
    uint16_t _channel;
    bool _enabled;
  };


  /**
   * \brief Аналоговый вход
   */
  class AnalogInput : public __AnalogBase, protected etl::forward_link<0>
  {
  public:
    /**
     * \brief Конструктор аналогового входа по ссылке
     * \param device Устройство с аналоговым входом
     * \param channel Номер дискретного входа
     */
    AnalogInput(device::iDevice& device, uint16_t channel);

    /**
     * \brief Конструктор аналогового входа по укзаателю
     * \param device Устройство с аналоговым входом
     * \param channel Номер аналогового входа
     */
    AnalogInput(device::iDevice* device, uint16_t channel)
    : AnalogInput{*device, channel}
    {
    }

    //! Нулевой указатель на устройство запрещен
    AnalogInput(nullptr_t, uint16_t) =delete;

    //! Возвращает время, прошедшее с обновления состояния входа
    int32_t time_since_update() const;
    //! Считывает значение аналогового входа из привязанного устройства
    void scan();

  private:
    using list_t = etl::intrusive_list<AnalogInput, etl::forward_link<0>>;
    inline static list_t _list;

    friend list_t;
    friend void AIO_Scan();
  };


  /**
   * \brief Аналоговый выход
   */
  class AnalogOutput : public __AnalogBase, protected etl::forward_link<0>
  {
  public:
    /**
     * \brief Конструктор аналогового выхода по ссылке
     * \param device Устройство с аналоговым выходом
     * \param channel Номер аналогового выхода
     * \param value Значение выхода по умолчанию
     */
    AnalogOutput(device::iDevice& device, uint16_t channel, float value=0);

    /**
     * \brief Конструктор аналогового выхода по указателю
     * \param device Устройство с аналоговым выходом
     * \param channel Номер аналогового выхода
     * \param value Значение выхода по умолчанию
     */
    AnalogOutput(device::iDevice* device, uint16_t channel, float value=0)
    : AnalogOutput{*device, channel, value}
    {
    }

    //! Нулевой указатель на устройство запрещен
    AnalogOutput(nullptr_t, uint16_t, float) =delete;

    /**
     * \brief Записывает в аналоговый выход значение с плавающей точкой
     * \param value Новое значение
     * 
     * \retval true  Новое значение успешно записано
     * \retval false Произошла ошибка записи нового значения
    */
    bool set(float value);

    /**
     * \brief Оператор присвоения значения аналоговому выходу
     * \tparam T Тип переменной присваеваемого значения
     * \details
     * Преобразовывает значение в подходящий для аналогового выхода тип данных
     * перед записью
     */
    template<typename T>
    AnalogOutput& operator=(const T& value)
    {
      set(static_cast<float>(value));
      return *this;
    }

    //! Возвращает время, прошедшее с обновления состояния выхода
    int32_t time_since_update() const;
    //! Записывает значение выхода в привязанное устройство
    void update();

  private:
    using list_t = etl::intrusive_list<AnalogOutput, etl::forward_link<0>>;

    inline static list_t _list;

    friend list_t;
    friend void AIO_Update();
  };


  /**
   * \brief Массив аналоговых входов
   * \tparam N Размер массива
   * \tparam DEVICE_T Класс устройства с аналоговыми входами
   */
  template<size_t N>
  class AnalogInputArray : public ChannelArray<AnalogInput, N>
  {
  public:
    /**
     * \brief Конструктор массива аналоговых входов по ссылке на устройство
     * \param device Устройство с аналоговыми входами
     * \param start_channel Номер первого канала аналогового входа в массиве
     */
    AnalogInputArray(device::iDevice& device, uint16_t start_channel=0)
    : ChannelArray<AnalogInput, N>{device, start_channel}
    {
    }

    /**
     * \brief Конструктор массива аналоговых входов по указателю на устройство
     * \param device Указатель на устройство с аналоговыми входами
     * \param start_channel Номер первого канала аналогового входа в массиве
     */
    AnalogInputArray(device::iDevice* device, uint16_t start_channel=0)
    : AnalogInputArray{*device, start_channel}
    {
    }

    AnalogInputArray(nullptr_t, uint16_t) =delete;
  };


  /**
   * \brief Массив аналоговых выходов
   * \tparam N Размер массива
   * \tparam DEVICE_T Класс устройства с аналоговыми выходами
   */
  template<size_t N>
  class AnalogOutputArray : public ChannelArray<AnalogOutput, N>
  {
  public:
    /**
     * \brief Конструктор массива аналоговых выходов по ссылке на устройство
     * \param device Устройство с аналоговыми выходами
     * \param start_channel Номер первого канала аналогового выхода в массиве
     */
    AnalogOutputArray(device::iDevice& device, uint16_t start_channel=0)
    : ChannelArray<AnalogOutput, N>{device, start_channel}
    {
    }

    /**
     * \brief Конструктор массива аналоговых выходов по указателю на устройство
     * \param device Указатель на устройство с аналоговыми выходами
     * \param start_channel Номер первого канала аналогового выхода в массиве
     */
    AnalogOutputArray(device::iDevice* device, uint16_t start_channel=0)
    : AnalogOutputArray{*device, start_channel}
    {
    }

    AnalogOutputArray(nullptr_t, uint16_t) =delete;
  };


  /**
   * \brief Функция считывания значений аналоговых входов из привязанных
   * устройств
   * \details
   * Выполняется автоматически перед циклом вызова POU
   */
  void AIO_Scan();

  /**
   * \brief Функция записи значений аналоговых выходов в привязанные устройства
   * \details
   * Выполняется автоматически после цикла вызова POU
   */
  void AIO_Update();

  /**
   * \brief Проверка на равенство значения аналогового входа/выхода с переменной
   * \param analog Аналоговый вход/выход
   * \param value Сравниваемая переменная
   * \details
   * \attention
   * Проверку на равеноство не рекомендуется делать для чисел с плавающей
   * точкой, т.к. такие числа практически никогда не совпадут. Для чисел
   * с плавающей точкой рекомендуется использовать проверки "больше", "меньше",
   * "больше или равно" и "меньше или равно"
   */
  template<typename T>
  bool operator==(const __AnalogBase& analog, const T& value)
  {
    return analog.value() == static_cast<float>(value);
  }


  /**
   * \brief Проверка на неравенство аналогового входа/выхода
   * со значением переменной
   */
  template<typename T>
  bool operator!=(const __AnalogBase& analog, const T& value)
  {
    return analog.value() != static_cast<float>(value);
  }


  /**
   * \brief Проверка "больше"
   */
  template<typename T>
  bool operator>(const __AnalogBase& analog, const T& value)
  {
    return analog.value() > static_cast<float>(value);
  }


  /**
   * \brief Проверка "больше или равно" для аналогового входа/выхода слева
   */
  template<typename T>
  bool operator>=(const __AnalogBase& analog, const T& value)
  {
    return analog.value() >= static_cast<float>(value);
  }


  /**
   * \brief Проверка "меньше" для аналогового входа/выхода слева
   */
  template<typename T>
  bool operator<(const __AnalogBase& analog, const T& value)
  {
    return analog.value() < static_cast<float>(value);
  }


  /**
   * \brief Проверка "меньше или равно" для аналогового входа/выхода слева
   */
  template<typename T>
  bool operator<=(const __AnalogBase& analog, const T& value)
  {
    return analog.value() <= static_cast<float>(value);
  }


  /**
   * \}
   * \noop plc_aio
   */
}
