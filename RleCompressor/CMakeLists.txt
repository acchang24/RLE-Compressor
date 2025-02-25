#Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add .cpp files here
set(SOURCE_FILES RleData.cpp RleFile.cpp)
# Add .h files here
set(HEADER_FILES RleData.h RleFile.h)

# Create a library for rle compression
add_library(RleCompressor ${SOURCE_FILES} ${HEADER_FILES})
