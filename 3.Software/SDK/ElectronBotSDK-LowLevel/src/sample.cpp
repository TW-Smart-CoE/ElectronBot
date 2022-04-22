#include "electron_low_level.h"
#include <opencv2/videoio/legacy/constants_c.h>
#include <thread>
#include "USBInterface.h"

using namespace cv;
using namespace std;

int main()
{
    // int USB_VID = 0x1001;
    // int USB_PID = 0x8023;
    // int devNum = USB_ScanDevice(USB_PID, USB_VID);

    // while(true) {
    //     printf("devNum: %d\n", devNum);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //     devNum = USB_ScanDevice(USB_PID, USB_VID);
    // }

    ElectronLowLevel robot;

    while (!robot.Connect()) {
        robot.Disconnect();
        printf("No Robot Connected!\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    printf("Robot connected!\n");

    VideoCapture video("video.mp4");
    Mat frame;


    while (true)
    {
        video >> frame;
        if (frame.empty())
            break;

        robot.SetImageSrc(frame);
        robot.Sync();
    }

    robot.Disconnect();
    printf("File play finished, robot Disconnected!\n");
    printf("Press [Enter] to exit.\n");

    getchar();
    return 0;
}

