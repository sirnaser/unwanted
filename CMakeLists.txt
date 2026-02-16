cmake_minimum_required(VERSION 3.16)
project(Undaunted LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(undaunted
    main.cpp
    src/Types.h
    src/Config.h
    src/Cell.h src/Cell.cpp
    src/Agent.h src/Agent.cpp
    src/Card.h src/Card.cpp
    src/Deck.h src/Deck.cpp
    src/Player.h src/Player.cpp
    src/Board.h src/Board.cpp
    src/GameEngine.h src/GameEngine.cpp
    src/GameState.h
    src/MainWindow.h src/MainWindow.cpp
)

target_link_libraries(undaunted PRIVATE Qt6::Core Qt6::Widgets)

# Copy resources to build directory
add_custom_command(TARGET undaunted POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:undaunted>/resources
)
