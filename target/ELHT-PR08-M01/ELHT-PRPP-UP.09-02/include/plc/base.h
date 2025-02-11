/**
 * \file plc/base.h
 * \brief Базовые определения библиотеки ПЛК
 */

#pragma once

#include <cstdint>
#include "device.h"
#include "etl/span.h"
#include "etl/intrusive_list.h"

namespace plc
{
  //! Тип ссылки связанного списка компонентов
  using component_link = etl::bidirectional_link<0>;

  //! Связанный список компонентов типа T
  template<typename T>
  using component_list = etl::intrusive_list<T, component_link>;
}

