# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (deps)


if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	deps/glfw-3.1.2/include/
	deps/glm-0.9.7.1/
	deps/glew-1.13.0/include/
	deps/stb/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

SOURCE_GROUP(src REGULAR_EXPRESSION ".*/src/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )








add_executable(triangle 
	src/01-triangle/main.cpp
	src/utils/shader.cpp
	src/utils/shader.hpp
	
	src/01-triangle/res/fragment.shader
	src/01-triangle/res/vertex.shader
)
target_link_libraries(triangle
	${ALL_LIBS}
)
file(COPY "${CMAKE_SOURCE_DIR}/src/01-triangle/res/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res/" )




add_executable(texture 
	src/02-texture/main.cpp
	src/utils/shader.cpp
	src/utils/shader.hpp
	src/utils/texture_loader.cpp
	src/utils/texture_loader.hpp
	
	src/02-texture/res/fragment.shader
	src/02-texture/res/vertex.shader
)
target_link_libraries(texture
	${ALL_LIBS}
)

file(COPY "${CMAKE_SOURCE_DIR}/src/02-texture/res/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res/" )





add_executable(multi-texture 
	src/03-multi-texture/main.cpp
	src/utils/shader.cpp
	src/utils/shader.hpp

	src/utils/texture_loader.cpp
	src/utils/texture_loader.hpp
	
	src/03-multi-texture/res/fragment.shader
	src/03-multi-texture/res/vertex.shader
)
target_link_libraries(multi-texture
	${ALL_LIBS}
)

file(COPY "${CMAKE_SOURCE_DIR}/src/03-multi-texture/res/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res/" )




add_executable(controls 
	src/04-controls/main.cpp
	src/utils/shader.cpp
	src/utils/shader.hpp

	src/utils/texture_loader.cpp
	src/utils/texture_loader.hpp

	src/utils/controls.cpp
	src/utils/controls.hpp
	
	src/04-controls/res/fragment.shader
	src/04-controls/res/vertex.shader
)
target_link_libraries(controls
	${ALL_LIBS}
)

file(COPY "${CMAKE_SOURCE_DIR}/src/04-controls/res/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res/" )


