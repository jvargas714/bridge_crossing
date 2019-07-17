project(testing)
include_directories(../third_party/libyaml-dist-0.2.2/include)
include_directories(../src)

add_executable(testing_crossing_processor
        ../src/BridgeCrossingHandler.cpp
        ../src/BridgeCrossingProcessor.cpp
        ../src/BridgeCrossingHandler.h
        ../src/BridgeCrossingProcessor.h
        ../src/hiker.cpp
        ../src/hiker.h
        test_BridgeCrossingProcessor.cpp)
target_link_libraries(testing_crossing_processor yaml)