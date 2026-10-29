cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(HELLOWORLD_VERSION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${HELLOWORLD_VERSION})

include(CTest)
include(FetchContent)

if(BUILD_TESTING)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        SYSTEM
    )
    FetchContent_MakeAvailable(googletest)
endif()

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)

target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(helloworld_cli PRIVATE
    helloworld
)

if(BUILD_TESTING)
    add_executable(test_version test_version.cpp)

    target_link_libraries(
        test_version
        PRIVATE
            helloworld
            GTest::gtest_main
    )
    include(GoogleTest)
    gtest_discover_tests(test_version)
endif()

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    if(BUILD_TESTING)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(BUILD_TESTING)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR DEB)
elseif(APPLE)
    set(CPACK_GENERATOR TGZ)
endif()
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
