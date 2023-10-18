cmake_minimum_required(VERSION 3.10)
project(
	"INF2705 - TP3"
	LANGUAGES
	CXX
)

add_executable(tp3)

add_subdirectory(src)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
)
target_link_libraries(tp3
	${SDL2_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

add_compile_options(
	-Wall
	-Wextra
	-pedantic
	-O2
	-std=c++17
)

set_target_properties(tp3
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin
)
