/**
 * \file kernel/kernel_base.h
 * \brief Базовые определения библиотеки ядра
 */

#pragma once

#include "FreeRTOS.h"
#include "elh/error_handler.h"
#include "elh/access_port.h"
#include "etl/array.h"
#include "etl/span.h"
#include "etl/variant_pool.h"
#include "etl/optional.h"
#include "etl/delegate.h"
#include "etl/circular_buffer.h"
#include "etl/string.h"
#include "etl/string_stream.h"
#include "etl/string_utilities.h"
#include "etl/intrusive_list.h"

// См. \ref kernel_config
#if defined(__has_include)
  #if __has_include("kernel_config.h")
    #include "kernel_config.h"
  #else
    #include "kernel_config_template.h"
  #endif
#else
#include "kernel_config.h"
#endif

//! Ядро
namespace kernel
{

}
