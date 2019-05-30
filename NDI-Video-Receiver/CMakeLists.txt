cmake_minimum_required(VERSION 3.5)
project(NDI-Video-Receiver)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

find_package(PkgConfig)
pkg_check_modules(NDI REQUIRED libndi)
include_directories(${NDI_INCLUDE_DIRS})
link_directories(${NDI_LIBRARY_DIRS})

pkg_check_modules(ZBAR REQUIRED zbar)
include_directories(${ZBAR_INCLUDE_DIRS})
link_directories(${ZBAR_LIBRARY_DIRS})

add_executable(${PROJECT_NAME}
        ConfigRead.cpp
        NdiCom.cpp
        NdiVideoReceiver.cpp
)
target_link_libraries(${PROJECT_NAME} ${NDI_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ZBAR_LIBRARIES})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
if(OpenCV_FOUND)
    # インクルードパスを指定
    include_directories(${OpenCV_INCLUDE_DIRS} )

    # リンクするライブラリ指定
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
endif(OpenCV_FOUND)
