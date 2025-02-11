add_library(${PROJECT_NAME}_examples OBJECT 01_inbuilt_inputs_outputs/01_doutput_blink.cpp
                                            01_inbuilt_inputs_outputs/02_dinput_doutput.cpp
                                            01_inbuilt_inputs_outputs/03_latching_relay.cpp
                                            02_terminal/01_terminal_output.cpp
                                            02_terminal/02_terminal_input.cpp
                                            02_terminal/03_array_output.cpp
                                            03_time_functions/01_delay_using_systicks.cpp
                                            03_time_functions/02_delay_using_ton.cpp
                                            03_time_functions/03_delay_using_tof.cpp
                                            03_time_functions/04_get_date.cpp
                                            03_time_functions/05_set_date.cpp
                                            04_expansion_modules/01_module_input.cpp
                                            04_expansion_modules/02_modules_input_output.cpp
                                            04_expansion_modules/03_on_off_regulator.cpp
                                            04_expansion_modules/04_setup_dinput.cpp
                                            04_expansion_modules/05_setup_analog_input.cpp
                                            04_expansion_modules/06_reset_modules.cpp
                                            04_expansion_modules/07_setup_pwm.cpp
                                            04_expansion_modules/08_module_save_settings.cpp
                                            05_modbus_rtu_master/01_modbus_master_read.cpp
                                            05_modbus_rtu_master/02_modbus_master_write.cpp
                                            05_modbus_rtu_master/03_modbus_ecd1.cpp
                                            05_modbus_rtu_master/04_modbus_ecd2.cpp
                                            05_modbus_rtu_master/05_modbus_ecv1.cpp
                                            05_modbus_rtu_master/06_modbus_master_buffer_auto_poll.cpp
                                            06_modbus_rtu_slave/01_modbus_slave.cpp
                                            07_retain_memory/01_retain_memory.cpp
                                            08_serial_port/01_serial_port_read.cpp
                                            08_serial_port/02_serial_port_write.cpp
                                            08_serial_port/03_serial_port_write_read.cpp
            )
target_include_directories(${PROJECT_NAME}_examples PUBLIC include)
target_link_libraries(${PROJECT_NAME}_examples PUBLIC ${PROJECT_NAME})
