cmake_minimum_required(VERSION 3.26)
project(Agenda_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Agenda_Project
        main.c
        timer.c timer.h
        list.c list.h
        cell.c cell.h
        agenda.c agenda.h
        files.c
        files.h)
