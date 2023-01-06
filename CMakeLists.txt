cmake_minimum_required(VERSION 3.24)

project(cpsc-585)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)

#-------------------------------------------------------------------------------
# https://github.com/adishavit/argh/releases/tag/v1.3.1
include_directories(SYSTEM lib/argh-1.3.1/)

#-------------------------------------------------------------------------------
# https://glad.dav1d.de/
add_subdirectory(lib/glad)
set(LIBRARIES ${LIBRARIES} glad)

#-------------------------------------------------------------------------------
# https://www.glfw.org/

# Turn off building their docs/tests/examples.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw-3.3.2)
set(LIBRARIES ${LIBRARIES} glfw)

#-------------------------------------------------------------------------------
# https://github.com/gurki/vivid/releases/tag/v2.2.1
include_directories(SYSTEM lib/vivid-2.2.1/include)
include_directories(SYSTEM lib/vivid-2.2.1/dependencies/glm)

#-------------------------------------------------------------------------------
# https://glm.g-truc.net/0.9.9/index.html
include_directories(SYSTEM lib/glm-0.9.9.7/)

#-------------------------------------------------------------------------------
# https://fmt.dev/latest/index.html
# Turn off building their tests
set(FMT_TES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/fmt-7.0.3)
set(LIBRARIES ${LIBRARIES} fmt::fmt)
include_directories(SYSTEM thirdparty/fmt-7.0.3/include)

find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_gl_LIBRARY})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	list(APPEND _453_CMAKE_CXX_FLAGS ${_453_CMAKE_CXX_FLAGS} "-Wall" "-pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	list(APPEND _453_CMAKE_CXX_FLAGS ${_453_CMAKE_CXX_FLAGS} "-fdiagnostics-color=always" "-Wall" "-pedantic" "-Wconversion")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	# using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/permissive-")
	add_compile_options("/Zc:preprocessor")
	# Remove the default warning level.
	# https://stackoverflow.com/questions/58708772/cmake-project-in-visual-studio-gives-flag-override-warnings-command-line-warnin
	string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	 # The above preprocessor warns about certain things in windows headers.
	 # Turn those off: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5105?view=vs-2019
	add_compile_options("/wd5105")

	set(_453_CMAKE_CXX_FLAGS ${_453_CMAKE_CXX_FLAGS} /W3)

endif()

if(UNIX)
	set(LIBRARIES ${LIBRARIES} pthread GL dl)
endif(UNIX)
if (WIN32)
endif()

# Compile our main application
file(GLOB SOURCES
        src/*
)

set(INCLUDES ${INCLUDES} src)

set(APP_NAME "cpsc-585")

# Copy all the shaders and tell the build system to re-run CMAKE if one of them changes
file(GLOB files src/shaders/*)
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	configure_file(${file} shaders/${name})
endforeach()

add_executable(${APP_NAME} ${SOURCES})
target_include_directories(${APP_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${APP_NAME} ${LIBRARIES})
target_compile_definitions(${APP_NAME} PRIVATE ${DEFINITIONS})
target_compile_options(${APP_NAME} PRIVATE ${_453_CMAKE_CXX_FLAGS})
set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH "./" BUILD_RPATH "./")
