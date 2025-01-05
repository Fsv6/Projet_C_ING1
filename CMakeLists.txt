cmake_minimum_required(VERSION 3.29)
project(Projet_C_ING1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Projet_C_ING1
    interpreter.c
    interpreter.h
    main.c
    axes_generator.c
        cursor_manager.c
        commands.c
        parser.c
)
