

add_executable(
    unittest
    test_lru_cache.cpp
    test_disjoint_sets.cpp
    test_concepts.cpp
    test_memory.cpp
    test_music_cache.cpp
    test_command.cpp
    main.cpp
)

target_link_libraries(
    unittest
    data_struct
    dummyplayer_lib
    gtest_main
)


add_test(NAME AllUnitTests COMMAND unittest)
