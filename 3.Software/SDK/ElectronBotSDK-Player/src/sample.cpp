#include "electron_player.h"


int main()
{
    ElectronPlayer robot;

    if (robot.Connect())
        printf("Robot connected!\n");
    else
    {
        printf("Connect failed!\n");
        getchar();
        return 0;
    }

    // robot.SetPose(ElectronPlayer::RobotPose_t{0, 30, 10, 0, 15, 0});
    printf("Robot Playing Video.\n");
    robot.Play("video.mp4");
    printf("Robot Play End.\n");

    robot.Disconnect();
    printf("Robot Disconnected.\n");

    getchar();
    return 0;
}

