# Подключение папок с CMake-библиотеками
#
# Для добавления библиотеки необходимо:
# - Добавить в данный файл команду "add_subdirectory()":
# add_subdirectory(название_папки)
#
# - Добавить в переменную PROJECT_LINK_CMAKE_LIBS в корневом CMakeLists
# название библиотеки из CMakeLists в папке с библиотекой:
# set(PROJECT_LINK_CMAKE_LIBS ulib
#                             название_библиотеки
#     )

add_subdirectory(ulib)
