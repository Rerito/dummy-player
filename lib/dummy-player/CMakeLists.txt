
add_library(dummyplayer_lib INTERFACE)

target_link_libraries(dummyplayer_lib INTERFACE data_struct)
target_include_directories(dummyplayer_lib INTERFACE "${CMAKE_SOURCE_DIR}/lib/dummy-player")
