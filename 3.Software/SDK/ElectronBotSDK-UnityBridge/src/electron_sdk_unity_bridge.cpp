#include "electron_sdk_unity_bridge.h"
#include "electron_low_level.h"
#include <opencv2/videoio/legacy/constants_c.h>

using namespace cv;

enum EmojiSrcType_t
{
    TYPE_NONE,
    TYPE_PICTURE,
    TYPE_VIDEO
};
EmojiSrcType_t emojiSrcType;
ElectronLowLevel robot;
Mat imgCamera;
Mat imgEmoji;
VideoCapture videoEmoji;
VideoCapture videoCamera;
int currentCameraIndex = -1;
int targetCameraIndex = 0;
bool showCameraOnRobot = false;

thread onUpdateTaskHandle;
bool isBusy = false;

uint8_t robotExtraData[32];
float robotJoints[6];

void OnUpdateTask(unsigned char* _imgDataEmoji, unsigned char* _imgDataCamera,
                  int _width, int _height, float* _setJoints, bool _enable)
{
#if 1
    bool rotobConnected = robot.isConnected;
    if (rotobConnected)
    {
        // Set extraData
        robot.SetJointAngles(_setJoints[0], _setJoints[1], _setJoints[2],
                             _setJoints[3], _setJoints[4], _setJoints[5], _enable);
    }
    switch (emojiSrcType)
    {
        case TYPE_PICTURE:
        {
            emojiSrcType = TYPE_NONE;

            if (!imgEmoji.empty())
            {
                if (rotobConnected) {
                    robot.SetImageSrc(imgEmoji);
                }

                flip(imgEmoji, imgEmoji, -1);
                //Resize Mat to match the array passed to it from C#
                resize(imgEmoji, imgEmoji, Size(_width, _height), cv::INTER_CUBIC);
                //Convert from RGB to ARGB
                cvtColor(imgEmoji, imgEmoji, COLOR_RGB2BGRA);
                memcpy(_imgDataEmoji, imgEmoji.data, imgEmoji.total() * imgEmoji.elemSize());
            }

            break;
        }
        case TYPE_VIDEO:
        {
            if (videoEmoji.get(CV_CAP_PROP_POS_FRAMES) < videoEmoji.get(CV_CAP_PROP_FRAME_COUNT))
            {
                videoEmoji >> imgEmoji;
                videoEmoji.set(CV_CAP_PROP_POS_FRAMES, videoEmoji.get(CV_CAP_PROP_POS_FRAMES) + 2);

                if (!imgEmoji.empty())
                {
                    if (rotobConnected) {
                        robot.SetImageSrc(imgEmoji);
                    }

                    flip(imgEmoji, imgEmoji, -1);
                    //Resize Mat to match the array passed to it from C#
                    resize(imgEmoji, imgEmoji, Size(_width, _height), cv::INTER_CUBIC);
                    //Convert from RGB to ARGB
                    cvtColor(imgEmoji, imgEmoji, COLOR_RGB2BGRA);
                    memcpy(_imgDataEmoji, imgEmoji.data, imgEmoji.total() * imgEmoji.elemSize());
                }
            }
            break;
        }
        case TYPE_NONE:
            break;
    }
    if (rotobConnected) {
        robot.Sync();
        robot.GetJointAngles(robotJoints);
    }

#else
    switch (emojiSrcType)
    {
        case TYPE_PICTURE:
        {
            emojiSrcType = TYPE_NONE;

            if (!imgEmoji.empty())
            {
                flip(imgEmoji, imgEmoji, -1);
                //Resize Mat to match the array passed to it from C#
                resize(imgEmoji, imgEmoji, Size(_width, _height), cv::INTER_CUBIC);
                //Convert from RGB to ARGB
                cvtColor(imgEmoji, imgEmoji, COLOR_RGB2BGRA);
                memcpy(_imgDataEmoji, imgEmoji.data, imgEmoji.total() * imgEmoji.elemSize());
            }

            break;
        }
        case TYPE_VIDEO:
        {
            if (videoEmoji.get(CV_CAP_PROP_POS_FRAMES) < videoEmoji.get(CV_CAP_PROP_FRAME_COUNT))
            {
                videoEmoji >> imgEmoji;

                videoEmoji.set(CV_CAP_PROP_POS_FRAMES, videoEmoji.get(CV_CAP_PROP_POS_FRAMES) + 2);

                if (!imgEmoji.empty())
                {
                    flip(imgEmoji, imgEmoji, -1);
                    //Resize Mat to match the array passed to it from C#
                    resize(imgEmoji, imgEmoji, Size(_width, _height), cv::INTER_CUBIC);
                    //Convert from RGB to ARGB
                    cvtColor(imgEmoji, imgEmoji, COLOR_RGB2BGRA);
                    memcpy(_imgDataEmoji, imgEmoji.data, imgEmoji.total() * imgEmoji.elemSize());
                }
            } else
            {
                emojiSrcType = TYPE_NONE;
            }

            break;
        }
    }
#endif

    if (videoCamera.isOpened() && currentCameraIndex == targetCameraIndex) {

        videoCamera >> imgCamera;
        if (!imgCamera.empty())
        {
            if (showCameraOnRobot)  {
                if (rotobConnected)
                    robot.SetImageSrc(imgCamera);
            }
            //flip horizontally
            flip(imgCamera, imgCamera, -1);
            //Resize Mat to match the array passed to it from C#
            resize(imgCamera, imgCamera, Size(_width, _height), cv::INTER_CUBIC);
            //Convert from RGB to ARGB
            cvtColor(imgCamera, imgCamera, COLOR_RGB2BGRA);
            memcpy(_imgDataCamera, imgCamera.data, imgCamera.total() * imgCamera.elemSize());
            if (showCameraOnRobot)  {
                memcpy(_imgDataEmoji, imgCamera.data, imgCamera.total() * imgCamera.elemSize());
            }
        }
    } else {
        if (currentCameraIndex != targetCameraIndex) {
            currentCameraIndex = targetCameraIndex;
            if (videoCamera.isOpened()) {
                videoCamera.release();
            }
            videoCamera.open(currentCameraIndex);
        }
    }

    isBusy = false;
}


void Native_OnKeyFrameChange(const char* _filePath)
{
    string s(_filePath);
    if (s.find(".mp4") != string::npos)
    {
        videoEmoji.release();
        videoEmoji = VideoCapture(_filePath);
        emojiSrcType = TYPE_VIDEO;
    } else if (s.find(".jpg") != string::npos ||
               s.find(".png") != string::npos ||
               s.find(".bmp") != string::npos)
    {
        imgEmoji.release();
        imgEmoji = imread(_filePath);
        emojiSrcType = TYPE_PICTURE;
    } else
    {
        emojiSrcType = TYPE_NONE;
    }
}


void Native_OnInit()
{
    robot.Connect();
}


float* Native_OnFixUpdate(unsigned char* _imgDataEmoji, unsigned char* _imgDataCamera,
                          int _width, int _height, float* _setJoints, bool _enable)
{
    if (!isBusy)
    {
        isBusy = true;

        onUpdateTaskHandle = std::thread(OnUpdateTask, _imgDataEmoji, _imgDataCamera,
                                         _width, _height, _setJoints, _enable);
        onUpdateTaskHandle.
        detach();
    }

    return robotJoints;
}


void Native_OnExit()
{
    if (videoCamera.isOpened()) {
        videoCamera.release();
    }

    if (videoEmoji.isOpened())
        videoEmoji.release();

    imgCamera.release();
    imgEmoji.release();

    robot.Disconnect();
}

void Native_OpenCamera(int index)
{
    // videoCamera.open(index);
    targetCameraIndex = index;
}

void Native_ShowCameraOnRobot(bool enable)
{
    showCameraOnRobot = enable;
}
