cmake_minimum_required(VERSION 3.26)
project(Agenda_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Agenda_Project
        main.c
        Source_Files/timer.c Source_Files/timer.h
        Source_Files/list.c Source_Files/list.h
        Source_Files/cell.c Source_Files/cell.h
        Source_Files/agenda.c Source_Files/agenda.h
        Source_Files/files.c
        Source_Files/files.h
        Source_Files/autocompletion.c
        Source_Files/autocompletion.h
        Complexity/complexity.c)
