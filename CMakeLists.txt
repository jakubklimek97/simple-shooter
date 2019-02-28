cmake_minimum_required(VERSION 3.12)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")


project(simple-shooter)
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "simple-shooter" )



set(SDL2_DIR "" CACHE PATH "SDL2 library")
set(SDL2_image_DIR "" CACHE PATH "SDL2image library")
set(glew_DIR "" CACHE PATH "glew library")
set(glm_DIR "" CACHE PATH "glm library")

file(GLOB ZRODLA 
"src/client/*.cpp"
"src/client/*.h"
"src/client/shaders/*.vs"
"src/client/shaders/*.fs")
add_executable(simple-shooter ${ZRODLA})
target_include_directories(simple-shooter 
PUBLIC "${SDL2_DIR}/include"
PUBLIC "${SDL2_image_DIR}/include"
PUBLIC "${glew_DIR}/include"
PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/glm"
)
target_link_directories(simple-shooter 
PUBLIC "${SDL2_DIR}/lib/x86"
PUBLIC "${SDL2_image_DIR}/lib/x86"
PUBLIC "${glew_DIR}/lib/Release/Win32"
)


target_link_libraries(simple-shooter 
SDL2.lib 
SDL2main.lib 
SDL2_image.lib
glew32.lib
opengl32.lib
)

file(COPY 
${SDL2_DIR}/lib/x86/SDL2.dll
${SDL2_image_DIR}/lib/x86/libjpeg-9.dll 
${SDL2_image_DIR}/lib/x86/libpng16-16.dll
${SDL2_image_DIR}/lib/x86/libtiff-5.dll
${SDL2_image_DIR}/lib/x86/libwebp-7.dll
${SDL2_image_DIR}/lib/x86/SDL2_image.dll
${SDL2_image_DIR}/lib/x86/zlib1.dll
${glew_DIR}/bin/Release/Win32/glew32.dll	
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
make_directory(${CMAKE_CURRENT_BINARY_DIR}/glm)
file(COPY ${glm_DIR}/glm DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/glm)
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB SHADERY "src/client/shaders/*.vs"
 "src/client/shaders/*.fs")
file(COPY ${SHADERY} DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})



