cmake_minimum_required(VERSION 3.10)
project(
	"INF2705 - TP4"
	LANGUAGES
	CXX C
)

add_executable(tp4)

add_subdirectory(src)
# Obscure option to avoid obscure warnign about multiple OpenGL installations
set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
)
target_link_libraries(tp4
	${SDL2_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

target_compile_options(
	tp4
	PUBLIC
		-Wall
		-Wextra
		-Wno-unused-parameter
		-Wvla
		-pedantic
		-O2
		-std=c++20
)

set_target_properties(tp4
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin
)
