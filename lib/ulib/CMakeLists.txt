cmake_minimum_required(VERSION 3.22)

project(ulib
        LANGUAGES C CXX
        DESCRIPTION "Пользовательская библиотека платформы AXCode"
        VERSION 0.1.0
        )

find_library(lib_path ${PROJECT_NAME} lib)
add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
set_property(TARGET ${PROJECT_NAME} PROPERTY IMPORTED_LOCATION ${lib_path})
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE plc)
