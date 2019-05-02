cmake_minimum_required(VERSION 3.8)
project(speech)

set(CMAKE_CXX_STANDARD 17)

set(RHVOICE_LIBS "-L/usr/src/RHVoice/build/linux/audio -L/usr/src/RHVoice/src/audio -L/usr/src/RHVoice/build/linux/core -L/usr/src/RHVoice/src/core -lRHVoice_core -lRHVoice_audio")
set(RHVOICE_DIR "/usr/local/include/RHVoice")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SPHINX REQUIRED sphinxbase)
pkg_check_modules(POCKETSPHINX REQUIRED pocketsphinx)

set(SOURCE_FILES main.cpp speaking.cpp speaking.h Recognition.cpp Recognition.h)

set(CMAKE_CXX_FLAGS "-Wall")

include_directories(${RHVOICE_DIR})
include_directories(${POCKETSPHINX_INCLUDE_DIRS})
include_directories(${SPHINX_INCLUDE_DIRS})

add_executable(speech ${SOURCE_FILES})

target_link_libraries(speech ${RHVOICE_LIBS})
target_link_libraries(speech ${POCKETSPHINX_LIBRARIES})
target_link_libraries(speech ${SPHINX_LIBRARIES})