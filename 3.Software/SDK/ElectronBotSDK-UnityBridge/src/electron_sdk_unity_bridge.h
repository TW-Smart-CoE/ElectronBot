#ifndef CPPTOUNITY_LIBRARY_H
#define CPPTOUNITY_LIBRARY_H

#if defined(__WIN32__)
#include <Windows.h>
#include <sdkddkver.h>
#endif
#include <iostream>

#include "IUnityInterface.h"

using namespace std;

#if defined(__WIN32__)
#define DLL_API extern "C" _declspec(dllexport)
#else
#define DLL_API extern "C" UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
#endif

DLL_API
void Native_OnKeyFrameChange(const char* _filePath);

DLL_API
float* Native_OnFixUpdate(unsigned char* _imgDataEmoji, unsigned char* _imgDataCamera,
                          int _width, int _height, float* _setJoints, bool _enable);

DLL_API
void Native_OnInit();

DLL_API
void Native_OnExit();

DLL_API
void Native_OpenCamera(int index);

DLL_API
void Native_ShowCameraOnRobot(bool enable);

#endif
