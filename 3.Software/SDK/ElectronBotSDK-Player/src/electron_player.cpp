#include "electron_player.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/legacy/constants_c.h>


bool ElectronPlayer::Connect()
{
    isConnected = lowLevelHandle->Connect();
    return isConnected;
}


bool ElectronPlayer::Disconnect()
{
    if (playTaskHandle.joinable())
        playTaskHandle.join();
    return lowLevelHandle->Disconnect();
}


void ElectronPlayer::Play(const std::string &_filePath)
{
    // Picture type
    if (_filePath.find(".jpg") != std::string::npos ||
        _filePath.find(".png") != std::string::npos ||
        _filePath.find(".bmp") != std::string::npos)
    {
        lowLevelHandle->SetImageSrc(cv::imread(_filePath));
        lowLevelHandle->Sync();
    }
        // Video type
    else if (_filePath.find(".mp4") != std::string::npos ||
             _filePath.find(".mov") != std::string::npos)
    {
        if (isConnected)
        {
            isPlaying = true;
            playTaskHandle = std::thread(PlayTask, this, _filePath, playSpeedRatio);
        }
    }
}


void ElectronPlayer::Play(const std::string &_filePath, float _speedRatio)
{
    playSpeedRatio = _speedRatio;
    Play(_filePath);
}


void ElectronPlayer::Stop()
{
    isPlaying = false;
}


void ElectronPlayer::SetPose(const ElectronPlayer::RobotPose_t &_pose)
{

}


ElectronPlayer::RobotPose_t ElectronPlayer::GetPose()
{
    return ElectronPlayer::RobotPose_t();
}


void ElectronPlayer::SetPlaySpeed(float _ratio)
{
    if (_ratio > 0)
    {
        playSpeedRatio = _ratio;
    }
}


void ElectronPlayer::PlayTask(ElectronPlayer* _obj, const std::string &_filePath, float _speedRatio)
{
    cv::VideoCapture video(_filePath);
    cv::Mat frame;

    auto totalFrameCount = video.get(CV_CAP_PROP_FRAME_COUNT);
    long index = 1;

    while (_obj->isPlaying && index < totalFrameCount)
    {
        video >> frame;
        index += (long) _speedRatio;
        video.set(CV_CAP_PROP_POS_FRAMES, index);

        _obj->lowLevelHandle->SetImageSrc(frame);
        _obj->lowLevelHandle->Sync();
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // printf("Played frame %d\n", index);
        // std::this_thread::sleep_for(std::chrono::seconds(_clientConfig.reconnectInterval));
    }

    _obj->isPlaying = false;
}

