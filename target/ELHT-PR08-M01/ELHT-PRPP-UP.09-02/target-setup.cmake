# Настройки компиляции таргета
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(DEBUG_COMPILE_OPTIONS -g0)
else()
  set(DEBUG_COMPILE_OPTIONS -g3)
endif()

add_compile_options(
  -march=rv32imafc
  -mabi=ilp32f
  -specs=nano.specs
  -msmall-data-limit=8
  -mno-save-restore
  -fdata-sections
  -ffunction-sections
  -fstack-usage
  -fdump-tree-optimized
  -fcallgraph-info=su
  -Wall
  -Werror=return-type
  -Werror=narrowing
  -Os
  -flto=auto
  ${DEBUG_COMPILE_OPTIONS}
)
add_compile_definitions()
add_link_options(
  -T${CMAKE_CURRENT_LIST_DIR}/ld/memory.ld
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -march=rv32imafc
  -mabi=ilp32f
  -specs=nano.specs
  -msmall-data-limit=8
  -mno-save-restore
  -nostartfiles
  -Wl,--gc-sections
  -Xlinker
  -print-memory-usage
  -Xlinker
)
