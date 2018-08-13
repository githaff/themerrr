cmake_minimum_required (VERSION 2.9)

project ("themerrr")

set (CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -pedantic")

if (NOT DEFINED CMAKE_INSTALL_BINDIR)
  set (CMAKE_INSTALL_BINDIR "bin")
endif ()

add_subdirectory ("src")
