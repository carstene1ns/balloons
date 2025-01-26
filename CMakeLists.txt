cmake_minimum_required(VERSION 3.20)

project("balloons" VERSION 0.3.0 LANGUAGES CXX
	HOMEPAGE_URL https://github.com/carstene1ns/balloons)

set(CMAKE_CXX_STANDARD 14)

include(GNUInstallDirs)

option(WANT_ASAN "build with adress sanitizer" OFF)
if(WANT_ASAN)
	add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
	add_link_options(-fno-omit-frame-pointer -fsanitize=address)
endif()

# game

add_executable(balloons
	src/game.cpp
	src/game.h
	src/game_world.cpp
	src/game_world.h
	src/main.cpp
	src/main.h
	src/map.cpp
	src/map.h
	src/object.h
	src/object_animate.cpp
	src/object_animate.h
	src/object_moving.cpp
	src/object_moving.h
	src/player.cpp
	src/player.h
	src/scoreList.cpp
	src/scoreList.h
	src/tiledef.h
	src/tiletab.h
	src/util.cpp
	src/util.h
)

target_compile_definitions(balloons PRIVATE VERSION=${PROJECT_VERSION})

option(WANT_SDL "Build SDL2 port" ON)
if(NOT WANT_SDL)
	message(FATAL_ERROR "Platform not available, try -DWANT_SDL=ON")
endif()

# game data

set(CMAKE_INSTALL_DATADIR "share/balloons")
GNUInstallDirs_get_absolute_install_dir(CMAKE_INSTALL_FULL_DATADIR CMAKE_INSTALL_DATADIR DATADIR)

option(WANT_DEVBUILD "Build for development" OFF)
if(NOT WANT_DEVBUILD)
	target_compile_definitions(balloons PRIVATE DATA_PREFIX="${CMAKE_INSTALL_FULL_DATADIR}")
endif()

# libs

if(WANT_SDL)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)

	target_link_libraries(balloons PRIVATE
		SDL2_mixer::SDL2_mixer
		SDL2_image::SDL2_image
		SDL2_ttf::SDL2_ttf
		SDL2::SDL2)
endif()

# install

install(TARGETS balloons RUNTIME)
install(DIRECTORY data/ TYPE DATA)
install(DIRECTORY doc/ TYPE DOC)
