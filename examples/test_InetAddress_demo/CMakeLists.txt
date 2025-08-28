set(SOURCE_FILES
    main.cc
)

add_executable(test_inet_addr_demo
    ${SOURCE_FILES}
)

target_link_libraries(test_inet_addr_demo PRIVATE MyMuduo)