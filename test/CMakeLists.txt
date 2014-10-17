
set(GAMEPLAY_SRC
    src/AbsoluteLayout.cpp
    src/AbsoluteLayout.h
    src/BoundingBox.h
    src/Transform.h
    src/Vector2.cpp
    src/Vector2.h
    src/lua/lua_ThemeUVs.h
    src/lua/lua_Touch.cpp
    src/lua/lua_Touch.h
    src/lua/lua_TouchTouchEvent.cpp
    src/lua/lua_TouchTouchEvent
)
include_directories( 
    src
    ../external-deps/lua/include
    ../external-deps/bullet/include
    ../external-deps/png/include
    ../external-deps/zlib/include
    ../external-deps/oggvorbis/include
    ../external-deps/openal/include
    ../external-deps/glew/include
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})
add_definitions(${GTK2_CFLAGS_OTHER})
add_definitions(-D__linux__)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

add_definitions(-lstdc++)

add_library(gameplay STATIC
    ${GAMEPLAY_SRC}
    ${GAMEPLAY_LUA}
)

set_target_properties(gameplay PROPERTIES
    OUTPUT_NAME "gameplay"
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${GAMEPLAY_VERSION}
)

source_group(lua FILES ${GAMEPLAY_LUA})
source_group(res FILES ${GAMEPLAY_RES} ${GAMEPLAY_RES} ${GAMEPLAY_RES_SHADERS} ${GAMEPLAY_RES_UI})
source_group(src FILES ${GAMEPLAY_SRC})



