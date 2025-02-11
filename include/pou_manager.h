/**
 * \file pou_manager.h
 * \brief Заголовочный файл настройки пользовательских POU
 * \details
 * Для доступа ко всем функциям Alpha-X CPU во все файлы с POU
 * должен быть включен данный файл, либо:
 * \code{.cpp}
 * #include "plc.h"
 * #include "ulib.h"
 * #include "system_api.h"
 * \endcode
 * 
 * После include в каждом файле следует прописать строку
 * \code{.cpp}
 * using namespace plc;
 * \endcode
 * 
 * Объявленные в данном файле типы, макросы, перечисления, переменные и функции 
 * будут доступны во всех файлах, включающих данный файл.
 */

#pragma once

#include "plc.h"
#include "ulib.h"
#include "system_api.h"



// Объявление функций POU из других файлов
void POU1(); // Функция POU из файла src/pou1.cpp
void POU2(); // Функция POU из файла src/pou2.cpp
void POU3(); // Функция POU из файла src/pou3.cpp
