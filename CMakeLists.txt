cmake_minimum_required(VERSION 3.2.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(LIBRARY_NAME allegro)

file(GLOB SOURCES "*.cpp" "*.c")
file(GLOB EMOJIC_DEPEND "*.emojic")

get_filename_component(MAIN_FILE main.emojic ABSOLUTE)
set(PACKAGE_FILE main.o)

include_directories(/usr/local/include)

add_library(${LIBRARY_NAME} STATIC ${SOURCES} ${PACKAGE_FILE})
set_property(TARGET ${LIBRARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_options(${LIBRARY_NAME} PUBLIC -Wall -Wno-unused-result -Wno-missing-braces -pedantic)
add_custom_command(OUTPUT ${PACKAGE_FILE} COMMAND emojicodec -p ${LIBRARY_NAME} -o ${PACKAGE_FILE} -i interface.emojii --color -r
                   -c ${MAIN_FILE} DEPENDS ${EMOJIC_DEPEND})

find_library(SYSTEM_ALLEGRO NAMES allegro)
target_link_libraries(allegro ${SYSTEM_ALLEGRO} allegro_color allegro_primitives allegro_image allegro_ttf allegro_audio allegro_acodec)
