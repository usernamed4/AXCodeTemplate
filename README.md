# Шаблон пользовательских программ AXCode User Template

## Описание

Шаблон для создания пользовательских программ.


Структура файлов шаблона:
- .vscode - Настройки VSCode
- build - Файлы скомпилированной программы
- include - Заголовочные файлы C/C++
  - pou_manager.h - Заголовочный файл настройки пользовательских POU. Должен быть включен в каждый файл с POU (#include "pou_manager.h")
- lib - Папка для пользовательских библиотек
- src - Папка для исходного кода C/C++
  - pou_manager.cpp - Файл настройки программируемого контроллера и POU
  - pou1.cpp, pou2.cpp, pou3.cpp - Преднастроенные файлы с пользовательскими POU
- target - Папка с таргетами программируемых контроллеров
  - ELH-PR08-M01 - Таргет Alpha-X CPU 01-1 00
  - utils - Утилиты работы с таргетами
    - AXCodeLoader.exe - Загрузчик программ в контроллер (для Windows)
    - AXCodeLoader - Загрузчик программ в контроллер (для Linux)
    - libmodbus.dll - Библиотека для загрузчика (для Windows)

## Использование

Для работы рекомендуется использовать среду разработки AXCode.
Скачать среду можно по ссылке https://elhart.ru/docs/info?id=9.

1. При первом запуске расширение CMake спросит используемый компилятор. В появившимся диалоговом окне выбрать "GCC xx.x.x riscv-none-elf", где xx.x.x - версия компилятора.


### Добавление POU

Для добавления отдельного файла с POU см. раздел "Добавление файлов с кодом" ниже.

Для добавления POU достаточно:
1. Создать функцию
   ```cpp
   void POU_NAME()
   {
     static bool init = false;
     if(init == false)
     {
       init = true;
       // Код, выполняемый один раз
       // ...
     }
     // Код, выполняемый периодически:
     // ...
   }
   ```
где `POU_NAME` - название функции POU

1. В файле include\pou_manager.h повторить объявление функции POU (без тела функции):
   ```cpp
   void POU_NAME();
   ```
2. В файле src/pou_manager.cpp найти функцию `PLC_MainSetup()` и добавить в нее вызов функции AddPOU с параметрами новой POU
   ```cpp
   void PLC_MainSetup()
   {
     AddPOU(POU_NAME, 100, 10);
   }
   ```
где 100 - период выполнения POU,
    10 - время ожидания сторожевого таймера POU.

### Добавление файлов с кодом

Для создания новых файлов с кодом необходимо:
1. Создать в папке "src" файл your_name.cpp (для кода на C++) или your_name.c (для кода на C), где your_name - название нового файла.
   *ВНИМАНИЕ! Функции программируемого контроллера доступны только для C++. Функции на C могут использоваться в файлах с кодом на C++*

2. Добавить в начало файла строки:
   ```cpp
   #include "pou_manager.h"
   using namespace plc;
   ```
3. Добавить собственные функции и переменные
4. Открыть файл CMakeLists.txt в корневом каталоге программы
5. Найти строки
```cmake
set(PROJECT_SOURCES 
    src/pou1.cpp
    src/pou2.cpp
    src/pou3.cpp
    # Добавьте сюда пути к файлам с исходным кодом

)
```
6. Добавить строку с относительныы путем к новому файлу 
```cmake
set(PROJECT_SOURCES 
    src/pou1.cpp
    src/pou2.cpp
    src/pou3.cpp
    # Добавьте сюда пути к файлам с исходным кодом
    src/your_name.cpp
)
```

### Компиляция программы

Для компиляции программы нажать F7.

Альтернативный вариант:
1. Нажать Shift+Ctrl+P
2. Ввести ">CMake: Build" и нажать Enter

### Загрузка программы

Порядок загрузки:
1. Подключите USB-кабель или преобразователь RS-485/USB к контроллеру
2. Определите через "Диспетчер устройств" номер появившегося при подключении COM-порта (для Windows)
3. В файле .vscode/tasks.json для задачи "load" укажите номер COM-порта в аргументах задачи:
```json
"tasks": [
    {
      "label": "load",
      "type": "shell",
      "windows": {
        "command": "target/utils/AXCodeLoader.exe",
        "args": [
          "COM1",  // <----- Номер COM-порта на Windows
          "-a",
          "1",
          "-b",
          "115200",
          "-p",
          "N",
          "-s",
          "1",
          "build/AXCODE_USER_PROGRAM.hex"
        ],
        "problemMatcher": []
      },
      "linux": {
        "command": "target/utils/AXCodeLoader",
        "args": [
          "/dev/ALPHAX", // <------- Название устройства на Linux
          "-a",
          "1",
          "-b",
          "115200",
          "-p",
          "N",
          "-s",
          "1",
          "build/AXCODE_USER_PROGRAM.hex"
        ],
        "problemMatcher": []
      }
    }
  ]
```
4. Нажмите F8 для начала загрузки.

Альтернативный вариант:
1. Нажать Shift+Ctrl+P
2. Ввести ">Tasks: Run Task" и нажать Enter
3. В появившемся списке выбрать "load"

Чтобы назначить свою горячую клавишу:
1. Нажать Shift+Ctrl+P
2. Ввести ">Preferences: Open Keyboard Shortcuts (JSON)" и нажать Enter
3. В открывшемся файле в квадратных скобках должна быть такая команда (добавить, если нет):
```json
// Place your key bindings in this file to override the defaultsauto[]
[
  { // Начало команды горячей клавиши для загрузки прошивки
   "key": "f8",
   "command": "workbench.action.tasks.runTask",
   "args": "load"
  } // Конец команды горячей клавиши для загрузки прошивки
]
```
где в key указана горячая клавиша, в args - название задачи для загрузки прошивки

### License information

This software contains both FOSS-licensed and non-FOSS-licensed files.
This software incorporates components from the projects listed below.

1. FreeRTOS, version 10.4.6, MIT license, Copyright (C) 2020 Amazon.com, Inc. or its affiliates (https://github.com/feilipu/Arduino_FreeRTOS_Library/blob/master/LICENSE) 
2. GNU ISO C++ Library (libstdc++), version 12.2.0, Copyright (C) 2009 Free Software Foundation, Inc. (https://www.fsf.org), GPL-3.0-with-GCC-exception license, (https://gcc.gnu.org/onlinedocs/libstdc++/manual/intro.html) 
3. Embedded Template Library, version 20.28.0, Copyright (c) 2016 jwellbelove, https://github.com/ETLCPP/etl , http://www.etlcpp.com, MIT license, (https://github.com/ETLCPP/etl/blob/master/LICENSE)
4. WCH Library (from MounRiver Studio), version 1.0.0, Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd., Apache 2.0 license, (http://mounriver.com/download  and https://www.wch-ic.com/downloads/category/30.html) 
5. ts-date, version 3.0.1, Copyright (c) 2017 standy, MIT license, (https://github.com/standy/ts-date )
