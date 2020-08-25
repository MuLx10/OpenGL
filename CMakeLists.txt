# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (OpenGL)

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





###################################################################################


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
add_custom_command(
   TARGET triangle POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/bin/triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/01-triangle/"
)


###################################################################################


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

add_custom_command(
   TARGET texture POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/bin/texture${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/02-texture/"
)



###################################################################################


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

add_custom_command(
   TARGET multi-texture POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/bin/multi-texture${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/03-multi-texture/"
)


###################################################################################


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

add_custom_command(
   TARGET controls POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/bin/controls${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/04-controls/"
)

###################################################################################


add_executable(3d-model 
	src/05-3d-model/main.cpp
	src/utils/shader.cpp
	src/utils/shader.hpp

	src/utils/3Dcontrols.cpp
	src/utils/3Dcontrols.cpp
	
	src/utils/obj_loader.cpp
	src/utils/obj_loader.hpp

	src/utils/dds_loader.cpp
	src/utils/dds_loader.hpp
	
	src/05-3d-model/res/fragment.shader
	src/05-3d-model/res/vertex.shader
)
target_link_libraries(3d-model
	${ALL_LIBS}
)

add_custom_command(
   TARGET 3d-model POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/bin/3d-model${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/05-3d-model/"
)


###################################################################################