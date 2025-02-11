# Примеры работы с пользовательской библиотекой платформы AXCode
cmake_minimum_required(VERSION 3.22)

add_library(${PROJECT_NAME}_examples src/fb_etriga_example.cpp
                                     src/fb_triggers_example.cpp
                                     src/fb_timers_example.cpp
            )
target_include_directories(${PROJECT_NAME}_examples PUBLIC include)
target_link_libraries(${PROJECT_NAME}_examples PUBLIC ulib)
