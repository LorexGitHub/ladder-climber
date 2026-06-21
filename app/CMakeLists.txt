cmake_minimum_required(VERSION 3.22)
project(DonkeyKong LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(DonkeyKong
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Platform.cpp
    src/Barrel.cpp
    src/DonkeyKong.cpp
)

target_compile_features(DonkeyKong PRIVATE cxx_std_20)
target_link_libraries(DonkeyKong PRIVATE SFML::Graphics)

# Sprite generator tool (builds once to create pixel-art PNGs)
add_executable(gen_sprites tools/gen_sprites.cpp)
target_link_libraries(gen_sprites PRIVATE SFML::Graphics)
add_custom_target(generate_sprites
    COMMAND gen_sprites
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    DEPENDS gen_sprites
)
add_dependencies(DonkeyKong generate_sprites)

# Copy a system font into assets/fonts/ as arial.ttf so text is visible
set(FONT_CANDIDATES
    /usr/share/fonts/TTF/DejaVuSans.ttf
    /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf
    /usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf)
foreach(F ${FONT_CANDIDATES})
    if(EXISTS "${F}")
        get_filename_component(FNAME "${F}" NAME)
        file(COPY "${F}" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/assets/fonts/")
        file(RENAME "${CMAKE_CURRENT_LIST_DIR}/assets/fonts/${FNAME}" "${CMAKE_CURRENT_LIST_DIR}/assets/fonts/arial.ttf")
        break()
    endif()
endforeach()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/bin/assets)
add_dependencies(copy_assets generate_sprites)
add_dependencies(DonkeyKong copy_assets)