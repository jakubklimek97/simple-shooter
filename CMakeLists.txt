cmake_minimum_required(VERSION 3.12)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")


project(simple-shooter)
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "simple-shooter" )

set(LIB_SDL2_DIR "" CACHE PATH "SDL2 library")
set(LIB_SDL2_image_DIR "" CACHE PATH "SDL2image library")
set(LIB_glew_DIR "" CACHE PATH "glew library")
set(LIB_glm_DIR "" CACHE PATH "glm library")
set(LIB_ASSIMP_INCLUDE_DIR " " CACHE PATH "Assimp include dir" )
set(LIB_ASSIMP_BINARY_DIR "" CACHE PATH "Assimp build results")
set(SDL2MIXERDIR "" CACHE PATH "SDL2_Mixer directory")
option(CONFIGURE_WITH_ASSIMP "Add assimp to project" OFF)
option(MOVE_SCRIPT_AUTO_CLOSE "Auto close file moving script" ON)
option(DOWNLOAD_ASSIMP "Download assimp to project" OFF)

file(GLOB ZRODLA 
"src/client/*.cpp"
"src/client/*.h"
"src/client/shaders/*.vs"
"src/client/shaders/*.fs")

add_executable(simple-shooter ${ZRODLA})
target_include_directories(simple-shooter 
PUBLIC "${LIB_SDL2_DIR}/include"
PUBLIC "${LIB_SDL2_image_DIR}/include"
PUBLIC "${LIB_glew_DIR}/include"
PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/glm"
PUBLIC "${LIB_ASSIMP_INCLUDE_DIR}"
)
target_link_directories(simple-shooter 
PUBLIC "${LIB_SDL2_DIR}/lib/x86"
PUBLIC "${LIB_SDL2_image_DIR}/lib/x86"
PUBLIC "${LIB_glew_DIR}/lib/Release/Win32"
PUBLIC "${LIB_ASSIMP_BINARY_DIR}"
)


target_link_libraries(simple-shooter 
SDL2.lib 
SDL2main.lib 
SDL2_image.lib
glew32.lib
opengl32.lib
)

file(COPY 
${LIB_SDL2_DIR}/lib/x86/SDL2.dll
${LIB_SDL2_image_DIR}/lib/x86/libjpeg-9.dll 
${LIB_SDL2_image_DIR}/lib/x86/libpng16-16.dll
${LIB_SDL2_image_DIR}/lib/x86/libtiff-5.dll
${LIB_SDL2_image_DIR}/lib/x86/libwebp-7.dll
${LIB_SDL2_image_DIR}/lib/x86/SDL2_image.dll
${LIB_SDL2_image_DIR}/lib/x86/zlib1.dll
${LIB_glew_DIR}/bin/Release/Win32/glew32.dll	
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(NOT ${CONFIGURE_WITH_ASSIMP})
message("Configuring without Assimp")
else()
file(GLOB ASSIMP_LIB RELATIVE "${LIB_ASSIMP_BINARY_DIR}" "${LIB_ASSIMP_BINARY_DIR}/*.lib")
message("Assimp found: ${ASSIMP_LIB}")
file(GLOB ASSIMP_DLL "${LIB_ASSIMP_BINARY_DIR}/*.dll" )
target_link_libraries(simple-shooter ${ASSIMP_LIB})
file(COPY ${ASSIMP_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

make_directory(${CMAKE_CURRENT_BINARY_DIR}/glm)
file(COPY ${LIB_glm_DIR}/glm DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/glm)
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB SHADERY "src/client/shaders/*.vs"
 "src/client/shaders/*.fs")
file(COPY ${SHADERY} DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})

file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}" MOVE_SCRIPT_BUILD_DIR)

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" MOVE_SCRIPT_SOURCE_DIR)

configure_file(src/moveNewFiles.bat.in moveNewFiles.bat)

##SDL2_NET HERE
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message("Setting up...")
    set(TEMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/Temp)
    set(LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
    file(MAKE_DIRECTORY ${TEMP_DIR})
    file(MAKE_DIRECTORY ${LIB_DIR})
    message("Downloading SDL2_net...")
    file(DOWNLOAD "https://www.libsdl.org/projects/SDL_net/release/SDL2_net-devel-2.0.1-VC.zip" ${TEMP_DIR}/SDL.zip) 
    message("Extracting SDL2_net...")
    execute_process(COMMAND ${CMAKE_COMMAND}  -E tar xf SDL.zip WORKING_DIRECTORY ${TEMP_DIR})
    file(REMOVE ${TEMP_DIR}/SDL.zip)
    file(GLOB LIB_SDLnet_DIR LIST_DIRECTORIES TRUE ${TEMP_DIR}/*)
    message("Copying SDL2_net...")
    execute_process(COMMAND ${CMAKE_COMMAND}  -E copy_directory ${LIB_SDLnet_DIR} lib/sdl2net/ WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    file(REMOVE_RECURSE ${LIB_SDLnet_DIR})
    set(SDL2_NET_PATH "${CMAKE_CURRENT_BINARY_DIR}/lib/sdl2net" CACHE STRING "SDL2_net lib file path" FORCE)
	find_package(SDL2_net REQUIRED)
	include_directories(${SDL2_NET_INCLUDE_DIRS})
list(GET SDL2_NET_LIBRARY 0 LIB_SDLnet_DIR)
	get_filename_component(LIB_SDLnet_DIR ${LIB_SDLnet_DIR} DIRECTORY)
	file(INSTALL ${LIB_SDLnet_DIR}/SDL2_net.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(simple-shooter ${SDL2_NET_LIBRARIES})

##SDL2_Mixer HERE

 message("Downloading SDL2_mixer...")
    file(DOWNLOAD "https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-VC.zip" ${TEMP_DIR}/SDL.zip) 
    message("Extracting SDL2_mixer...")
    execute_process(COMMAND ${CMAKE_COMMAND}  -E tar xf SDL.zip WORKING_DIRECTORY ${TEMP_DIR})
    file(REMOVE ${TEMP_DIR}/SDL.zip)
    file(GLOB LIB_SDLmixer_DIR LIST_DIRECTORIES TRUE ${TEMP_DIR}/*)
    message("Copying SDL2_mixer...")
    execute_process(COMMAND ${CMAKE_COMMAND}  -E copy_directory ${LIB_SDLmixer_DIR} lib/sdl2mixer/ WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    file(REMOVE_RECURSE ${LIB_SDLmixer_DIR})
    set(SDL2_MIXER_PATH "${CMAKE_CURRENT_BINARY_DIR}/lib/sdl2mixer" CACHE STRING "SDL2_mixer lib file path" FORCE)

find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(simple-shooter ${SDL2_MIXER_LIBRARIES})
list(GET SDL2_MIXER_LIBRARY 0 LIB_SDLmixer_DIR)
get_filename_component(LIB_SDLmixer_DIR ${LIB_SDLmixer_DIR} DIRECTORY)
file(INSTALL ${LIB_SDLmixer_DIR}/libFLAC-8.dll
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libmodplug-1.dll
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libmpg123-0.dll
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libogg-0.dll
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libopus-0.dll
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libopusfile-0.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libvorbis-0.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/libvorbisfile-3.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(INSTALL ${LIB_SDLmixer_DIR}/SDL2_mixer.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


##FreeType2
include(ExternalProject)
ExternalProject_Add(FreeType2 
	URL https://download.savannah.gnu.org/releases/freetype/freetype-2.10.0.tar.bz2
	PREFIX  ${TEMP_DIR}/FreeType2
	INSTALL_DIR  ${CMAKE_CURRENT_BINARY_DIR}/lib/ft2
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
ExternalProject_Add_Step(FreeType2 install)
ExternalProject_Get_Property(FreeType2 install_dir)
include_directories(${install_dir}/include/freetype2)
target_link_libraries(simple-shooter ${install_dir}/lib/freetyped.lib)



if(${DOWNLOAD_ASSIMP})
ExternalProject_Add(Assimp 
	URL https://github.com/assimp/assimp/archive/master.zip
	PREFIX  ${TEMP_DIR}/Assimp
	INSTALL_DIR  ${CMAKE_CURRENT_BINARY_DIR}/lib/assimp
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DASSIMP_BUILD_TESTS:BOOL=OFF -DASSIMP_BIN_INSTALL_DIR:PATH=lib -DCMAKE_BUILD_TYPE=Release
	BUILD_COMMAND ${CMAKE_COMMAND}  --build <BINARY_DIR> --target install --config Release
)
ExternalProject_Get_Property(Assimp install_dir)
set(LIB_ASSIMP_INCLUDE_DIR " ${install_dir}/include" CACHE PATH "Assimp include dir"  FORCE)
set(LIB_ASSIMP_BINARY_DIR "${install_dir}/lib" CACHE PATH "Assimp build results" FORCE)
#file(GLOB ASSIMP_LIB RELATIVE "${LIB_ASSIMP_BINARY_DIR}" "${LIB_ASSIMP_BINARY_DIR}/*.lib")
#message("Assimp found: ${ASSIMP_LIB}")
#file(GLOB ASSIMP_DLL "${LIB_ASSIMP_BINARY_DIR}/*.dll" )
#target_link_libraries(simple-shooter ${ASSIMP_LIB})
#file(COPY ${ASSIMP_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
message("Generate project and build Assimp project, then reconfigure again with CONFIGURE_WITH_ASSIMP")
endif()

if(NOT ${CONFIGURE_WITH_ASSIMP})
message("Configuring without Assimp")
else()
include_directories(${LIB_ASSIMP_INCLUDE_DIR})
file(GLOB ASSIMP_LIB RELATIVE "${LIB_ASSIMP_BINARY_DIR}" "${LIB_ASSIMP_BINARY_DIR}/*.lib")
message("Assimp found: ${ASSIMP_LIB}")
file(GLOB ASSIMP_DLL "${LIB_ASSIMP_BINARY_DIR}/*.dll" )
target_link_libraries(simple-shooter ${ASSIMP_LIB})
file(COPY ${ASSIMP_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()