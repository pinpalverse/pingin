cmake_minimum_required(VERSION 4.3)
project(pingin VERSION 1.0 DESCRIPTION "The pin HTTP/S engine" LANGUAGES C)
include(GNUInstallDirs)

# ---------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------
find_package(pinlog REQUIRED)
find_package(pinmem REQUIRED)

# RPATH – automatically finds both pinmem and pinlog
# ---------------------------------------------------------------------
set(CMAKE_SKIP_BUILD_RPATH FALSE)                    # keep build RPATH
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)             # use install RPATH during build
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)          # add linked directories

# Build-time: look in current build dir (for libpinmem) and in pinlog's location
set(CMAKE_BUILD_RPATH 
    "${CMAKE_CURRENT_BINARY_DIR}:$<TARGET_FILE_DIR:pinlog::pinlog>:${CMAKE_CURRENT_BINARY_DIR}:$<TARGET_FILE_DIR:pinmem::pinmem>"
)

# Install-time: look in the standard library install directory
set(CMAKE_INSTALL_RPATH 
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
)

# ---------------------------------------------------------------------
# Libraries
# ---------------------------------------------------------------------
add_executable(pingin main.c)


target_link_libraries(pingin PRIVATE pinlog)
target_link_libraries(pingin PRIVATE pinmem)

target_include_directories(pingin PRIVATE include/)
