find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(calculator
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    resources/icons.qrc
)

target_link_libraries(calculator PRIVATE Qt6::Widgets calculator_engine)

install(TARGETS calculator RUNTIME DESTINATION bin)
