
add_library(dummyplayer_lib INTERFACE)

target_include_directories(dummyplayer_lib INTERFACE "${CMAKE_SOURCE_DIR}/lib/dummy-player")
target_link_libraries(dummyplayer_lib data_struct)