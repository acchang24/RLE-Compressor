#Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add .cpp files here
set(SOURCE_FILES Main.cpp)
# Add .h files here
set(HEADER_FILES)

# rle executable target using source and header files
add_executable(rle ${SOURCE_FILES} ${HEADER_FILES})

# Link with RleCompressor library
target_link_libraries(rle RleCompressor)

# Build other subdirectories
add_subdirectory(RleCompressor)
add_subdirectory(Tests)
