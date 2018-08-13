aux_source_directory (. SRC_LIST)

find_package (X11 REQUIRED)
include_directories (${X11_INCLUDE_DIR})
link_directories (${X11_LIBRARIES})

add_executable (${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})

## Install rules
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
