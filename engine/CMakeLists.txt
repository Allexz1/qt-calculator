add_library(calculator_engine STATIC
    src/Tokenizer.cpp
    src/MathFunctions.cpp
    src/Parser.cpp
    src/MemoryRegister.cpp
    src/CalculatorEngine.cpp
)

target_include_directories(calculator_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(calculator_engine PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
