cmake_minimum_required(VERSION 3.10)
project(calc LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 1. Основная БИБЛИОТЕКА (было EXECUTABLE)
add_library(calc STATIC
    src/lexer.c
    src/parser.c
    src/evaluator.c
    src/tokens.c
    src/errors.c
    src/calculator.c
)

# 2. Публичные заголовки
target_include_directories(calc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 3. Пример использования (отдельный исполняемый файл)
add_executable(calc_example src/main.c)
target_link_libraries(calc_example calc m)

# 4. Тесты
enable_testing()

add_executable(test_lexer tests/test_lexer.c)
target_link_libraries(test_lexer calc m)
add_test(NAME LexerTest COMMAND test_lexer)

add_executable(test_parser tests/test_parser.c)
target_link_libraries(test_parser calc m)
add_test(NAME ParserTest COMMAND test_parser)

add_executable(test_evaluator tests/test_evaluator.c)
target_link_libraries(test_evaluator calc m)
add_test(NAME EvaluatorTest COMMAND test_evaluator)

add_executable(test_integration tests/test_integration.c)
target_link_libraries(test_integration calc m)
add_test(NAME IntegrationTest COMMAND test_integration)

# 5. Цель для запуска всех тестов
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_lexer test_parser test_evaluator test_integration
)
