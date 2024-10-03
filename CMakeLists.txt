cmake_minimum_required(VERSION 3.10)

project(gas VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(external/tgp)

add_executable(gas
    source/main.cpp
    source/gas_actions.cpp
    source/gas_engine.cpp
    source/gas_subengine.cpp
    source/gas_physics_engine.cpp
    source/gas_chemistry_engine.cpp
    source/gas_button_transitions.cpp
    source/gas_buttons.cpp
    source/gas_drawer.cpp
    source/gas_controller.cpp
    source/gas_chamber_window.cpp
    source/gas_graph.cpp
    source/gas_window_manager.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -Wall \
    -Wmissing-declarations \
    -Wcast-align \
    -Wcast-qual \
    -Wchar-subscripts \
    -Wconversion \
    -Wempty-body \
    -Wfloat-equal \
    -Wformat-nonliteral \
    -Wformat-security \
    -Wformat=2 \
    -Winline \
    -Wpacked \
    -Wpointer-arith \
    -Winit-self \
    -Wredundant-decls \
    -Wshadow \
    -Wstrict-overflow=2 \
    -Wswitch-default \
    -Wswitch-enum \
    -Wundef \
    -Wunreachable-code \
    -Wunused \
    -Wvariadic-macros \
    -Wno-nonnull \
    -Wno-missing-field-initializers \
    -Wno-narrowing \
    -Wno-varargs \
    -Wstack-protector \
    -fcheck-new \
    -fstack-protector \
    -fstrict-overflow \
    -fno-omit-frame-pointer \
    -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr"
)

include_directories(include)

target_link_libraries(gas PRIVATE tgp sfml-graphics sfml-window sfml-system)
