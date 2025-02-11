# Пользовательская программа AXCode

cmake_minimum_required(VERSION 3.22)

# Название проекта
set(PROJECT_NAME AXCODE_USER_PROGRAM)

# Выбор таргета
include(target/target-select.cmake)

project(${PROJECT_NAME}
        LANGUAGES C CXX ASM
        DESCRIPTION "Пользовательская программа AXCode"
        VERSION 1.0.0
        )

add_subdirectory(${AXCODE_TARGET_PATH})
add_subdirectory(lib)

# Пользовательские папки с заголовочными файлами
set(PROJECT_INCLUDES 
    include
)


# Пользовательские файлы с исходным кодом
set(PROJECT_SOURCES 
    src/pou_manager.cpp
    src/pou1.cpp
    src/pou2.cpp
    src/pou3.cpp
    # Добавьте сюда пути к файлам с исходным кодом

) 


# Внешние CMake-библиотеки
set(PROJECT_LINK_CMAKE_LIBS ulib
                            
)


# Команды для сборки прошивки (hex-файла)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} PUBLIC AXCode_CPU_target
                                             ${PROJECT_LINK_CMAKE_LIBS}
                      )

add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${PROJECT_NAME}.elf
  ${PROJECT_NAME}.hex
)
