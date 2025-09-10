set(SOURCE_FILES
    main.cpp
)

add_executable(server_demo
    ${SOURCE_FILES}
)

target_link_libraries(server_demo PRIVATE MyMuduo)