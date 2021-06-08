cmake_minimum_required(VERSION 3.14)
project(video-app C CXX)
set(CMAKE_CXX_STANDARD 14)


add_subdirectory(lib/glfw)
# add_subdirectory(lib/glm)


include_directories(lib/glad/include)
include_directories(lib/FFmpeg/include)
include_directories(lib/imgui/include)
link_directories(lib/FFmpeg/lib)

#add_definitions(_DGL_SILENCE_DEPRECATION)

# list(APPEND SOURCES
#     lib/glad/lib/glad.c

#     src/*.cpp
#     src/*.hpp
# )
FILE(GLOB ALL_SOURCE_FILE "src/*.cpp", "src/*.hpp", "lib/glad/lib/glad.c", "lib/imgui/src/*.cpp")
# aux_source_directory(./src DIR_SRCS)

list(APPEND EXTRA_LIBS
    "opengl32.lib"
)


add_executable(video-app ${ALL_SOURCE_FILE}) 

target_link_libraries(video-app glfw ${EXTRA_LIBS})
target_link_libraries(video-app avcodec.lib avdevice.lib avfilter.lib avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)