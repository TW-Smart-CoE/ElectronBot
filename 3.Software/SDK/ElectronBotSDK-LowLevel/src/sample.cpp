#include "electron_low_level.h"
#include <opencv2/videoio/legacy/constants_c.h>
#include <thread>
#include "USBInterface.h"

using namespace cv;
using namespace std;

int main()
{
#if 1
    ElectronLowLevel robot;

    while (!robot.Connect()) {
        robot.Disconnect();
        printf("No Robot Connected!\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    printf("Robot connected!\n");

    VideoCapture video;//("video.mp4");
    Mat frame;

    // if (!video.open("video.mp4")) {
    //     printf("file open failed!\n");
    // }
    if (video.isOpened()) {
        printf("file is opened!\n");
    } else {
        printf("file is not open!\n");
    }

    while (true)
    {
        video >> frame;
        if (frame.empty()){
            printf("video frame is empty!\n");
            robot.SetImageSrc(string("img.jpg"));
            robot.Sync();
            break;
        }
            

        robot.SetImageSrc(frame);
        robot.Sync();
    }
    
    for (int i = 0; i < 3; i++) {
        robot.SetImageSrc(string("img.png"));
        robot.Sync();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        robot.SetImageSrc(string("img.jpg"));
        robot.Sync();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    robot.Disconnect();
    printf("File play finished, robot Disconnected!\n");
    printf("Press [Enter] to exit.\n");
    getchar();
    return 0;
#else

    cv::Mat image1;
        cv::Mat image2;
    image1 = cv::imread("img.jpg");
    std::vector<cv::Mat> SplitChannel;
    cv::split(image1,SplitChannel);
    SplitChannel[1] == 0;
    cv::merge(SplitChannel,image2);
    cv::namedWindow("NO GREEN CHANNEL");
    cv::imshow("results",image2);
    cv::waitKey();
    return 0;
#endif
}

