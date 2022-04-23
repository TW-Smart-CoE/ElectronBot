cmake_minimum_required(VERSION 3.19)
project(ElectronBotSDK-Player)

set(CMAKE_CXX_STANDARD 14)

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

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        )

link_directories(
        lib
        D:\\Libraries\\Cpp\\OpenCV_lib\\x64\\vc16\\lib
        D:\\Libraries\\Cpp\\Boost_lib\\lib64-msvc-14.1
)

add_library(${PROJECT_NAME} SHARED
        "src/electron_player.cpp"
)

target_link_libraries(${PROJECT_NAME}
        ElectronBotSDK-LowLevel
        opencv_world348d.lib
)

add_executable(Sample
        "src/sample.cpp"
)

target_link_libraries(Sample
        ${PROJECT_NAME}
        ElectronBotSDK-LowLevel
        opencv_world348d.lib
)

elseif (CMAKE_HOST_APPLE)


include_directories(
        src
        lib
        ../_Libs/mac/lib
        # /opt/local/include/opencv4/
        ../_Libs/mac/include/opencv4/
        /opt/local/include/boost
)

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        )

link_directories(
        ../_Libs/mac/opencv4_lib/
        ../_Libs/mac/opencv4_lib/opencv4/3rdparty
        ../_Libs/mac/lib/
        ../build/ElectronBotSDK-LowLevel
        ../build/ElectronBotSDK-Player
)

add_library(${PROJECT_NAME} SHARED
        "src/electron_player.cpp"
)

target_link_libraries(${PROJECT_NAME}
        libusb-1.0.a
        libusb.a
        libUSBInterface.a
        libElectronBotSDK-LowLevel.a
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

add_executable(Sample_Player
        "src/sample.cpp"
)

target_link_libraries(Sample_Player
        ${PROJECT_NAME}
        libusb-1.0.a
        libusb.a
        libUSBInterface.a
        libElectronBotSDK-LowLevel.a
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

elseif (CMAKE_HOST_UNIX)

include_directories(
        src
        lib
        /home/uos_user/opensource/stages/include
        /home/uos_user/opensource/stages/include/libusb-1.0
        /home/uos_user/opensource/stages/include/opencv4
)

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        )

link_directories(
        /home/uos_user/opensource/stages/lib
        /home/uos_user/opensource/stages/lib/opencv4/3rdparty
        ../build/ElectronBotSDK-LowLevel
        ../build/ElectronBotSDK-Player
)

add_library(${PROJECT_NAME} SHARED
        "src/electron_player.cpp"
)

target_link_libraries(${PROJECT_NAME}
        libusb-1.0.a
        libusb.a
        libUSBInterface.a
        libElectronBotSDK-LowLevel.a
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

add_executable(Sample_Player
        "src/sample.cpp"
)

target_link_libraries(Sample_Player
        ${PROJECT_NAME}
        libusb-1.0.a
        libusb.a
        libUSBInterface.a
        libElectronBotSDK-LowLevel.a
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


add_dependencies(ElectronBotSDK-Player ElectronBotSDK-LowLevel)
add_dependencies(Sample_Player ElectronBotSDK-LowLevel ElectronBotSDK-Player)