cmake_minimum_required(VERSION 3.19)
project(ElectronBotSDK-UnityBridge)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

find_package(OpenCV 4.5.5 REQUIRED)
find_package(Boost 1.81.0 REQUIRED)

if (CMAKE_HOST_APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Foundation")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework CoreFoundation")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework IOKit")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Security -framework Accelerate -framework OpenCL -framework AVFoundation -framework CoreGraphics")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework CoreMedia -framework CoreVideo")
endif ()

if (CMAKE_HOST_WIN32)
include_directories(
        src
        lib
        D:\\Libraries\\Cpp\\OpenCV_lib\\include
        D:\\Libraries\\Cpp\\Boost_lib\\include
        D:\\Libraries\\Cpp\\Boost_lib
)

link_directories(
        lib
        D:\\Libraries\\Cpp\\OpenCV_lib\\x64\\vc16\\lib
        D:\\Libraries\\Cpp\\Boost_lib\\lib64-msvc-14.1
)


add_library(${PROJECT_NAME} SHARED
        src/electron_sdk_unity_bridge.cpp)

target_link_libraries(${PROJECT_NAME}
        ElectronBotSDK-LowLevel
        opencv_world348d.lib
)

elseif (CMAKE_HOST_APPLE)
        include_directories(
                src
                lib
                lib/unity
                ../_Libs/mac/lib
                ${OpenCV_INCLUDE_DIRS}
                ${Boost_INCLUDE_DIRS}
        )

link_directories(
        lib/mac
        # /opt/local/lib/
        ../_Libs/mac/lib/
        ../build/ElectronBotSDK-LowLevel
)


add_library(${PROJECT_NAME} SHARED
        src/electron_sdk_unity_bridge.cpp)

        target_link_libraries(${PROJECT_NAME}
                libElectronBotSDK-LowLevel.a
                libUSBInterface.a
                libusb-1.0.a
                libusb.a
                ${OpenCV_LIBRARIES}
        )

elseif (CMAKE_HOST_UNIX)

include_directories(
        src
        lib
        lib/unity
        /home/uos_user/opensource/stages/include
        /home/uos_user/opensource/stages/include/libusb-1.0
        /home/uos_user/opensource/stages/include/opencv4
)

link_directories(
        lib/mac
        # /opt/local/lib/
        # /opt/local/lib/opencv4/
        /home/uos_user/opensource/stages/lib
        /home/uos_user/opensource/stages/lib/opencv4/3rdparty
        ../build/ElectronBotSDK-LowLevel
        
)


add_library(${PROJECT_NAME} SHARED
        src/electron_sdk_unity_bridge.cpp)

target_link_libraries(${PROJECT_NAME}
        libElectronBotSDK-LowLevel.a
        libUSBInterface.a
        libusb-1.0.a
        libusb.a
        # opencv_world
        # opencv_core
        # opencv_imgcodecs
        # opencv_imgproc
        # opencv_videoio

        libopencv_core.a
        libopencv_imgcodecs.a
        libopencv_imgproc.a
        libopencv_videoio.a

        libIlmImf.a
        libade.a
        libittnotify.a
        liblibjpeg-turbo.a
        liblibopenjp2.a
        liblibpng.a
        liblibprotobuf.a
        liblibtiff.a
        liblibwebp.a
        libquirc.a
        libzlib.a
)
endif ()

add_dependencies(ElectronBotSDK-UnityBridge ElectronBotSDK-LowLevel)