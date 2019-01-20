
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(
    dummy-player
    main.cpp
    make_command_dispatcher.cpp
    music_types/track_metadata.cpp
    threads/status_thread.cpp
    threads/user_input_thread.cpp
    threads/command_dispatcher_thread.cpp
)

target_include_directories(dummy-player PRIVATE ${CMAKE_SOURCE_DIR}/app/dummy-player)

target_link_libraries(
    dummy-player
    Threads::Threads
    data_struct
    dummyplayer_lib
)
