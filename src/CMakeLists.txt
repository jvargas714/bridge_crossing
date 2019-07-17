project(bridge_crossing)

include_directories(../third_party/libyaml-dist-0.2.2/include)

SET(SOURCE_FILES
        main.cpp
        BridgeCrossingProcessor.h
        BridgeCrossingProcessor.cpp
        hiker.h
        hiker.cpp
        bridge_crossing_types.h
        bridge_crossing_exceptions.h
        BridgeCrossingHandler.h
        BridgeCrossingHandler.cpp)

add_executable(bridge_crossing ${SOURCE_FILES})
target_link_libraries(bridge_crossing yaml)


