#Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES Main.cpp RleData.cpp RleFile.cpp)
set(HEADER_FILES RleData.h RleFile.h)

add_executable(rle ${SOURCE_FILES} ${HEADER_FILES})
