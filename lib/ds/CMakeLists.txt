
add_library(data_struct INTERFACE)

set(DATA_STRUCT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/ds")
target_include_directories(data_struct INTERFACE "${DATA_STRUCT_SOURCE_DIR}")

