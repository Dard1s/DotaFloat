file(GLOB_RECURSE LOADER_SRC *.cpp *.hpp)

add_executable(dotafloat_loader ${LOADER_SRC})

target_include_directories(dotafloat_loader PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/shared
)

target_link_libraries(dotafloat_loader PRIVATE
    psapi
)

target_compile_definitions(dotafloat_loader PRIVATE
    DOTAFLOAT_LOADER=1
    NOMINMAX
)

set_target_properties(dotafloat_loader PROPERTIES
    WIN32_EXECUTABLE FALSE
)

if(MSVC)
    target_link_options(dotafloat_loader PRIVATE /SUBSYSTEM:CONSOLE)
endif()
