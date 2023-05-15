cmake_minimum_required(VERSION 3.19)
project(ElectronBotSDK-LowLevel)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 4.5.5 REQUIRED)
find_package(Boost 1.81.0 REQUIRED)

if (CMAKE_HOST_APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Foundation")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework CoreFoundation")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework IOKit")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Security -framework Accelerate -framework OpenCL -framework AVFoundation -framework CoreGraphics")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework CoreMedia -framework CoreVideo")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -arch arm64 ")


# include(CheckCCompilerFlag)

# set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;x86_64")
# check_c_compiler_flag("-arch x86_64" x86_64Supported)
# message("x86_64Supported=${x86_64Supported}")

# set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;arm64")
# check_c_compiler_flag("-arch arm64" arm64Supported)
# message("arm64Supported=${arm64Supported}")
# message("PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
endif ()

if (CMAKE_HOST_WIN32)
        include_directories(
                src
                lib

                # D:\\Libraries\\Cpp\\OpenCV_lib\\include
                # D:\\Libraries\\Cpp\\Boost_lib\\include
                # D:\\Libraries\\Cpp\\Boost_lib  
        )
elseif (CMAKE_HOST_APPLE)
        include_directories(
                src
                lib
                ../_Libs/mac/lib
                ${OpenCV_INCLUDE_DIRS}
                ${Boost_INCLUDE_DIRS}
        )
elseif (CMAKE_HOST_UNIX)
include_directories(
        src
        /home/uos_user/opensource/stages/include
        /home/uos_user/opensource/stages/include/libusb-1.0
        /home/uos_user/opensource/stages/include/opencv4
        #/opt/local/include/boost
)
endif ()

file(GLOB_RECURSE SOURCES
        "src/electron_low_level.cpp"
        )

if (CMAKE_HOST_WIN32)
link_directories(
        lib
        D:\\Libraries\\Cpp\\OpenCV_lib\\x64\\vc16\\lib
        D:\\Libraries\\Cpp\\Boost_lib\\lib64-msvc-14.1
)
elseif (CMAKE_HOST_APPLE)
link_directories(
        ../_Libs/mac/lib/
        # /opt/local/lib/
        # /opt/local/lib/opencv4/
        ../build/ElectronBotSDK-LowLevel
)
elseif (CMAKE_HOST_UNIX)
link_directories(
        /home/uos_user/opensource/stages/lib
        /home/uos_user/opensource/stages/lib/opencv4/3rdparty
        /usr/lib/x85_64-linux-gnu
        /usr/lib
        # /opt/local/lib/
        # /opt/local/lib/opencv4/
        ../build/ElectronBotSDK-LowLevel
)
endif ()

add_library(
        ${PROJECT_NAME} 
        STATIC
        "src/electron_low_level.cpp"
)

# add_library(
#         ${PROJECT_NAME}_static
#         STATIC
#         "src/electron_low_level.cpp"
# )

add_executable(Sample_LowLevel
        "src/sample.cpp"
)

if (CMAKE_HOST_WIN32)
target_link_libraries(${PROJECT_NAME}
        USBInterface
        opencv_world348d.lib
)

target_link_libraries(Sample_LowLevel
        ${PROJECT_NAME}
        opencv_world348d.lib
)
elseif(CMAKE_HOST_APPLE)
        target_link_libraries(${PROJECT_NAME}
                libusb-1.0.a
                libusb.a
                libUSBInterface.a
                ${OpenCV_LIBRARIES}
        )

        # target_link_libraries(${PROJECT_NAME}.a
        # # libusb-1.0.a
        # libusb.a
        # libUSBInterface.a
        # opencv_world
        # )
        target_link_libraries(Sample_LowLevel
                ${PROJECT_NAME}
                libusb-1.0.a
                libusb.a
                libUSBInterface.a
                ${OpenCV_LIBRARIES}
        )
elseif (CMAKE_HOST_UNIX)

target_link_libraries(${PROJECT_NAME}
        libusb-1.0.a
        libusb.a
        libUSBInterface.a
        # opencv_world

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

        libopencv_imgcodecs.a
        libopencv_imgproc.a
        libopencv_videoio.a
        libopencv_core.a

)
# target_link_libraries(${PROJECT_NAME}.a
#         # libusb-1.0.a
#         libusb.a
#         libUSBInterface.a
#         opencv_world
# )

target_link_libraries(Sample_LowLevel
        ${PROJECT_NAME}
        pthread
        dl
        m
        rt
        libUSBInterface.a
        libusb.a
        libusb-1.0.a
        # opencv_core
        # opencv_imgcodecs
        # opencv_imgproc
        # opencv_videoio


        libopencv_video.a
        libopencv_videoio.a
        libopencv_imgcodecs.a
        libopencv_imgproc.a
        # libopencv_highgui.a
        libopencv_core.a

        libavcodec.a
        libavdevice.a
        libavfilter.a
        libavformat.a
        libavresample.a
        libavutil.a
        libswresample.a
        libswscale.a

        libIlmImf.a
        libade.a
        libittnotify.a
        liblibjpeg-turbo.a
        liblibopenjp2.a
        liblibjasper.a
        liblibpng.a
        liblibprotobuf.a
        liblibtiff.a
        liblibwebp.a
        libquirc.a
        libzlib.a

        # gdk_pixbuf-2.0
        # libgdk-x11-2.0.a
        # libgtk-x11-2.0.a
        dc1394
        udev

)
endif ()

add_dependencies(Sample_LowLevel ElectronBotSDK-LowLevel)