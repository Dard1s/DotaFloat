file(GLOB_RECURSE CORE_SRC *.cpp *.hpp)

add_library(dota_core SHARED ${CORE_SRC})

target_include_directories(dota_core PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/external/imgui
)

target_link_libraries(dota_core PRIVATE
    d3d11
    dxgi
    psapi
    winmm
)

target_compile_definitions(dota_core PRIVATE
    DOTAFLOAT_INTERNAL=1
    DOTAFLOAT_VERSION=0x024A1B
)

set_target_properties(dota_core PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
