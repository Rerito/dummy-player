if(MSVC)
	set(gtest_force_shared_crt ON)
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparties/googletest)
