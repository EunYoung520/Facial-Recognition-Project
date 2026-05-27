cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(CMakeProject1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

set(OpenCV_DIR "C:/vcpkg/installed/x64-windows/share/opencv4")
set(dlib_DIR "C:/vcpkg/installed/x64-windows/share/dlib")
set(Protobuf_DIR "C:/vcpkg/installed/x64-windows/share/protobuf")
set(flatbuffers_DIR "C:/vcpkg/installed/x64-windows/share/flatbuffers")

find_package(OpenCV REQUIRED)
find_package(dlib CONFIG REQUIRED)

add_executable(CMakeProject1
    CMakeProject1.cpp
)

target_link_libraries(CMakeProject1 PRIVATE
    ${OpenCV_LIBS}
    dlib::dlib
)
