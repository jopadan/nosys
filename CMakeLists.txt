cmake_minimum_required( VERSION 3.5 )

project( NoSYS VERSION 0.1 LANGUAGES CXX )
set(PROJECT_DESCRIPTION "C++ platform framework backend abstration")

include(GNUInstallDirs)

if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
endif()

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
add_compile_options(-march=native -O3)
add_compile_options(-DHAVE_GLCOREARB_H=1)
add_compile_options(-D_FILE_OFFSET_BITS=64)
add_compile_options(-fdata-sections)
add_compile_options(-fpermissive)
add_compile_options(-ffunction-sections)
add_compile_options(-Wall)
#add_compile_options(-Wextra)
add_compile_options(-Wpedantic)
add_link_options(-Wl,--gc-sections)
#add_link_options(-Wl,--print-gc-sections)
add_link_options(-Wl,-s)

find_package(PkgConfig)
find_package(OpenGL)
find_package(WLH)
pkg_check_modules(GLFW IMPORTED_TARGET REQUIRED glfw3)
pkg_check_modules(GLC  IMPORTED_TARGET REQUIRED quesoglc)
pkg_check_modules(SAIL IMPORTED_TARGET REQUIRED sail-c++)

add_library( NoSYS INTERFACE )
target_include_directories( NoSYS INTERFACE
        PUBLIC_HEADER $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<INSTALL_INTERFACE:${GLFW_INCLUDE_DIRS}>
	$<INSTALL_INTERFACE:${GLC_INCLUDE_DIRS}>
	$<INSTALL_INTERFACE:${SAIL_INCLUDE_DIRS}>
)

add_executable( example examples/example.cpp )
target_link_libraries( example NoSYS OpenGL::OpenGL OpenGL::GLU PkgConfig::GLFW PkgConfig::GLC PkgConfig::SAIL)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/OpenNMA DESTINATION
	${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/NoSYS DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers
)
