cmake_minimum_required(VERSION 3.5)
project(NDI-Video-Switch-Sender)
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
        ConfigRead.h
        NDI-Switch-Sender.cpp
        NDISender.cpp
        NDISender.h
        NdiVideoSender.config
        Realsense.cpp
        Realsense.h
        USBCamera.cpp
        USBCamera.h
        VideoSource.cpp
        VideoSource.h
        VideoSourceManager.cpp
        VideoSourceManager.h)
target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${NDI_LIBRARIES})
if(OpenCV_FOUND)
    # インクルードパスを指定
    include_directories(${OpenCV_INCLUDE_DIRS} )

    # リンクするライブラリ指定
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
endif(OpenCV_FOUND)
