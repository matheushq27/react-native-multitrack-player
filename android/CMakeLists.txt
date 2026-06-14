cmake_minimum_required(VERSION 3.22.1)
project(MultiTrackPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SUPERPOWERED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Superpowered")

add_library(
  MultiTrackPlayer
  SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/JNI_MultiTrackPlayer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../cpp/Track.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../cpp/MultiTracksPlayer.cpp
  ${SUPERPOWERED_DIR}/OpenSource/SuperpoweredAndroidAudioIO.cpp
)

target_include_directories(
  MultiTrackPlayer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../cpp
  ${SUPERPOWERED_DIR}
  ${SUPERPOWERED_DIR}/OpenSource
)

target_link_libraries(
  MultiTrackPlayer
  log
  android
  OpenSLES
  ${SUPERPOWERED_DIR}/libSuperpoweredAndroid${ANDROID_ABI}.a
)
