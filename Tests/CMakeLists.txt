#Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add .cpp files here
set(SOURCE_FILES Tests.cpp)
# Add .h files here
set(HEADER_FILES md5.h catch.hpp)

# Include RleCompressor headers
include_directories(../RleCompressor)

# rle executable target using source and header files
add_executable(tests ${SOURCE_FILES} ${HEADER_FILES})

# Link with RleCompressor library
target_link_libraries(tests RleCompressor)
