cmake_minimum_required(VERSION 3.10)

project(input-example)

add_executable(${PROJECT_NAME} main.c)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

target_link_libraries(${PROJECT_NAME} PRIVATE
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	SDL2main
)

if(PLATFORM_PSP)
	create_pbp_file(
		TARGET ${PROJECT_NAME}
		ICON_PATH NULL
		BACKGROUND_PATH NULL
		PREVIEW_PATH NULL
		TITLE ${PROJECT_NAME}
	)
endif()

file(COPY PressStart2P.ttf DESTINATION ${CMAKE_BINARY_DIR})
file(COPY square_sans_serif_7.ttf DESTINATION ${CMAKE_BINARY_DIR})
