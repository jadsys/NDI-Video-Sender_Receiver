cmake_minimum_required(VERSION 3.5)
project(NDI-Video-Sender)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(realsense2 REQUIRED)
find_package(Threads REQUIRED)

find_package(PkgConfig)
pkg_check_modules(NDI REQUIRED libndi)
include_directories(${NDI_INCLUDE_DIRS})
link_directories(${NDI_LIBRARY_DIRS})


add_executable(${PROJECT_NAME}
        ConfigRead.cpp
        NDI-Switch-Sender.cpp
        NDISender.cpp
        Realsense.cpp
        USBCamera.cpp
        VideoSource.cpp
        VideoSourceManager.cpp)
target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${NDI_LIBRARIES})
if(OpenCV_FOUND)
    # インクルードパスを指定
    include_directories(${OpenCV_INCLUDE_DIRS} )

    # リンクするライブラリ指定
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
endif(OpenCV_FOUND)
