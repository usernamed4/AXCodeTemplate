# Выбор таргета
set(AXCODE_TARGET_PATH ${CMAKE_CURRENT_LIST_DIR}/ELHT-PR08-M01/ELHT-PRPP-UP.09-02)

# Подключение таргета
set(CMAKE_TOOLCHAIN_FILE ${AXCODE_TARGET_PATH}/toolchain.cmake)
include(${AXCODE_TARGET_PATH}/target-setup.cmake)
