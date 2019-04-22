cmake_minimum_required(VERSION 3.0)
add_executable(Dot_server_test Server_test.cpp)
add_executable(Dot_client_test Client_test.cpp)

target_compile_features(Dot_server_test PRIVATE cxx_enum_forward_declarations)
target_compile_features(Dot_client_test PRIVATE cxx_enum_forward_declarations)

target_include_directories(Dot_server_test PRIVATE Dot)
target_include_directories(Dot_client_test PRIVATE Dot)

target_link_libraries(Dot_server_test Dot)
target_link_libraries(Dot_client_test Dot)

add_test(DotServer_test ${CMAKE_BINARY_DIR}/Dot_server_test)
add_test(DotClient_test ${CMAKE_BINARY_DIR}/Dot_client_test)
