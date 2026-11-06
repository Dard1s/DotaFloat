if(WIN32)
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

if(ENABLE_LTO AND MSVC)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

add_compile_definitions(
    $<$<CONFIG:Release>:NDEBUG>
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
)
