cmake_minimum_required(VERSION 3.0)
project(Dot VERSION 0.3.3 LANGUAGES CXX DESCRIPTION "Asynchronous Networking library for C++")
find_package(Threads)
find_package(clog)
find_package(comm)
find_package(EventManager)
find_package(crossguid)
file(GLOB_RECURSE Dot_HDRS "include/*.hpp")
file(GLOB_RECURSE Dot_SRCS "src/*.cpp")

#configure_file( ${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

add_library(Dot ${Dot_HDRS} ${Dot_SRCS})
target_compile_features(Dot PRIVATE cxx_enum_forward_declarations)
target_link_libraries(Dot ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Dot ${CLOG_LIBS})
target_link_libraries(Dot ${COMM_LIBS})
target_link_libraries(Dot crossguid)
if(WIN32)
	target_link_libraries(Dot wsock32 ws2_32)
endif()
set_target_properties(Dot PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(Dot PUBLIC include)
target_include_directories(Dot PUBLIC ${CLOG_INCLUDE_DIR})
target_include_directories(Dot PUBLIC ${COMM_INCLUDE_DIR})
target_include_directories(Dot PUBLIC ${EM_INCLUDE_DIR})

install(FILES DotConfig.cmake DESTINATION lib/cmake/Dot)
install(FILES ${Dot_HDRS} DESTINATION include/Dot)
install(TARGETS Dot
        PUBLIC_HEADER DESTINATION include/Dot
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)


add_executable(Dot_server_test test/Server_test.cpp)
add_executable(Dot_client_test test/Client_test.cpp)

target_compile_features(Dot_server_test PRIVATE cxx_enum_forward_declarations)
target_compile_features(Dot_client_test PRIVATE cxx_enum_forward_declarations)

target_include_directories(Dot_server_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Dot_client_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Dot_server_test Dot)
target_link_libraries(Dot_client_test Dot)

add_test(DotServer_test ${CMAKE_BINARY_DIR}/Dot_server_test)
add_test(DotClient_test ${CMAKE_BINARY_DIR}/Dot_client_test)
