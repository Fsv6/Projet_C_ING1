cmake_minimum_required(VERSION 3.29)
project(Projet_C_ING1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Projet_C_ING1
    commands_parser.c
    commands_parser.h
    main.c
    axes_generator.c
        commands_manager/commands.c
        commands_manager/python_functions_writer.c
        commands_manager/bloc_commands_manager.c
)
