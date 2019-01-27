add_avr_executable(
    main
    main.cpp
)

target_link_libraries(
    main-${AVR_MCU}.elf
)
