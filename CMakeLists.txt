cmake_minimum_required(VERSION 3.14)
project(shimeji)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-window sfml-system sfml-graphics)

add_compile_options("-W")
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.*pp)
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system sfml-audio sfml-graphics sfml-window ${X11_LIBRARIES})