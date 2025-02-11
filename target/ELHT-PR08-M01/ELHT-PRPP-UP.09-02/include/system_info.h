/**
 * \file system_info.h
 * \brief Информация о системе и прошивке
 */

#pragma once

#include "etl/string.h"
#include "time_menu/time_struct.h"


/**
 * \brief Возвращает время загрузки прошивки процессорного модуля
 * \return Время загзруки прошивки в секундах с 01.01.1970г с учетом часового
 * пояса
 */
time_menu::time_type SystemInfo_GetFirmwareUploadUnixTime();

/**
 * \brief Возвращает структуру времени и даты загрузки прошивки процессорного
 * модуля
 */
time_menu::TimeStruct SystemInfo_GetFirmwareUploadTimeStruct();

/**
 * \brief Возвращает строку с временем и датой загрузки прошивки процессорного
 * модуля
 */
etl::string<20> SystemInfo_GetFirmwareUploadTimeString();


