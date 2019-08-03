cmake_minimum_required(VERSION 3.5.1)
project(Dot VERSION 0.3.3 LANGUAGES CXX DESCRIPTION "Asynchronous Networking library for C++")

find_package(Threads)
find_package(clog)
find_package(crosssocket)
find_package(comm)
find_package(EventManager)
find_package(crossguid)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Set the build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

file(GLOB_RECURSE DOT_HDRS "include/*.hpp")
file(GLOB_RECURSE DOT_SRCS "src/*.cpp")

add_library(Dot ${DOT_SRCS})
set_property(TARGET Dot PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(Dot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

#dependencies
target_link_libraries(Dot PUBLIC ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(Dot PRIVATE clog::clog)

target_link_libraries(Dot PRIVATE comm::comm)

target_link_libraries(Dot PRIVATE EventManager)

target_link_libraries(Dot PRIVATE crossguid)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNINGS "-Werror" "-Wall")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(WARNINGS "-Werror" "-Wall")
elseif(MSVC)
    set(WARNINGS "/WX" "/W4")
endif()
target_compile_options(Dot PRIVATE ${WARNINGS})
target_compile_features(Dot PRIVATE cxx_enum_forward_declarations)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	include(GNUInstallDirs)

	set(DOT_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
	set(DOT_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
	set(DOT_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
	set(DOT_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
	set(DOT_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")

	set(DOT_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/Dot/cmake")
	set(DOT_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/Dot")

	# Install target
	install(TARGETS Dot EXPORT DotTargets
		RUNTIME       DESTINATION ${DOT_RUNTIME_INSTALL_DIR}
		LIBRARY       DESTINATION ${DOT_LIBRARY_INSTALL_DIR}
		ARCHIVE       DESTINATION ${DOT_ARCHIVE_INSTALL_DIR}
		FRAMEWORK     DESTINATION ${DOT_FRAMEWORK_INSTALL_DIR})

	# Install headers
	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
		DESTINATION ${DOT_INC_INSTALL_DIR})

	# Make cmake config files for all targets
	install(EXPORT  DotTargets
		DESTINATION ${DOT_CMAKE_CONFIG_INSTALL_DIR}
		FILE DotConfig.cmake)

	# Install readme and license
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
		DESTINATION ${DOT_ADDITIONAL_FILES_INSTALL_DIR})
endif()

if (DOT_TESTS)
  add_subdirectory(test)
endif()
