cmake_minimum_required(VERSION 3.12)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")


project(simple-shooter)
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "simple-shooter" )



set(LIB_SDL2_DIR "" CACHE PATH "SDL2 library")
set(LIB_SDL2_image_DIR "" CACHE PATH "SDL2image library")
set(LIB_glew_DIR "" CACHE PATH "glew library")
set(LIB_glm_DIR "" CACHE PATH "glm library")
set(LIB_ASSIMP_INCLUDE_DIR CACHE PATH "Assimp include dir" )
set(LIB_ASSIMP_BINARY_DIR CACHE PATH "Assimp build results")
option(MOVE_SCRIPT_AUTO_CLOSE "Auto close file moving script" ON)

file(GLOB ZRODLA 
"src/client/*.cpp"
"src/client/*.h"
"src/client/shaders/*.vs"
"src/client/shaders/*.fs")
file(GLOB ASSIMP_LIB RELATIVE "${LIB_ASSIMP_BINARY_DIR}" "${LIB_ASSIMP_BINARY_DIR}/*.lib")
message("Assimp found: ${ASSIMP_LIB}")
file(GLOB ASSIMP_DLL "${LIB_ASSIMP_BINARY_DIR}/*.dll" )
add_executable(simple-shooter ${ZRODLA})
target_include_directories(simple-shooter 
PUBLIC "${LIB_SDL2_DIR}/include"
PUBLIC "${LIB_SDL2_image_DIR}/include"
PUBLIC "${LIB_glew_DIR}/include"
PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/glm"
PUBLIC "${ASSIMP_INCLUDE_DIR}"
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
make_directory(${CMAKE_CURRENT_BINARY_DIR}/glm)
file(COPY ${LIB_glm_DIR}/glm DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/glm)
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB SHADERY "src/client/shaders/*.vs"
 "src/client/shaders/*.fs")
file(COPY ${SHADERY} DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})

file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}" MOVE_SCRIPT_BUILD_DIR)

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" MOVE_SCRIPT_SOURCE_DIR)

configure_file(src/moveNewFiles.bat.in moveNewFiles.bat)



