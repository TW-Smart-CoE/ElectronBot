using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine.UI;

public class UnityGetImageFromCpp : MonoBehaviour
{
    public GameObject texturePlaneEmoji;
    public GameObject texturePlaneCamera;
    public int textureWidth = 512;
    public int textureHeight = 512;
    public Dropdown cameraDropDown;

    private Texture2D mTexEmoji;
    private Color32[] mPixel32Emoji;
    private GCHandle mPixelHandleEmoji;
    private IntPtr mPixelPtrEmoji;
    private Texture2D mTexCamera;
    private Color32[] mPixel32Camera;
    private GCHandle mPixelHandleCamera;
    private IntPtr mPixelPtrCamera;

    public RobotController robot;
    public PoseEditor poseEditor;
    public bool showCameraOnRobot;


    // #if UNITY_EDITOR

    [DllImport("ElectronBotSDK-LowLevel")]
    private static extern void LowLevel_OnInit();

    [DllImport("ElectronBotSDK-UnityBridge")]
    private static extern void Native_OnKeyFrameChange(string argString);

    [DllImport("ElectronBotSDK-UnityBridge")]
    private static extern IntPtr Native_OnFixUpdate(IntPtr dataEmoji, IntPtr dataCamera, int width, int height,
        IntPtr setJoints, bool enable);

    [DllImport("ElectronBotSDK-UnityBridge")]
    private static extern void Native_OnExit();

    [DllImport("ElectronBotSDK-UnityBridge")]
    private static extern void Native_OnInit();

    [DllImport("ElectronBotSDK-UnityBridge")]
    private static extern void Native_OpenCamera(int index);

    [DllImport("ElectronBotSDK-UnityBridge")]
    private static extern void Native_ShowCameraOnRobot(bool enable);


    // Use this for initialization
    void Start()
    {
        try
        {
            LowLevel_OnInit();
        }
        catch (EntryPointNotFoundException e)
        {
            Logger.Instance.LogError("LowLevel_OnInit can't be load");
        }
        Logger.Instance.Log("UnityGetImageFromCpp Start");
        FindWebCams();
        OpenCamera();
        // while (!RunExecutable.is_camera_opened) ;
        try {
            Native_OnInit();
        } catch (EntryPointNotFoundException e) {
            Logger.Instance.LogError("Native_OnInit can't be load");
        }
        InitTexture();
        texturePlaneEmoji.GetComponent<Renderer>().material.mainTexture = mTexEmoji;
        texturePlaneCamera.GetComponent<Renderer>().material.mainTexture = mTexCamera;
    }

    IEnumerator OpenCamera() { 
        yield return Application.RequestUserAuthorization(UserAuthorization.WebCam);
        if (Application.HasUserAuthorization(UserAuthorization.WebCam))
        {
            Logger.Instance.Log("Camera Authorized");
        } else {
            Logger.Instance.Log("Camera Unauthorized");
        }
    }
    void FindWebCams()
    {
        cameraDropDown.ClearOptions();
        Logger.Instance.Log("Finding WebCams");
        List<string> cameraList = new List<string>();
        foreach (var device in WebCamTexture.devices)
        {
            Logger.Instance.Log("WebCam Name: " + device.name);
            cameraList.Add(device.name);
        }
        cameraDropDown.AddOptions(cameraList);
    }

    public void CameraDropDown_OnValueChanged()
    {
        Logger.Instance.Log(string.Format("Camera Index {0} Selected", cameraDropDown.value));
        Native_OpenCamera(cameraDropDown.value);
    }
    public void CameraImage_OnClick()
    {
        showCameraOnRobot = !showCameraOnRobot;
        Native_ShowCameraOnRobot(showCameraOnRobot);
    }

    // Update is called once per frame
    void Update()
    {
        // UpdateTexture();
    }


    void InitTexture()
    {
        mTexEmoji = new Texture2D(textureWidth, textureHeight, TextureFormat.ARGB32, false);
        mPixel32Emoji = mTexEmoji.GetPixels32();
        //Pin pixel32 array
        mPixelHandleEmoji = GCHandle.Alloc(mPixel32Emoji, GCHandleType.Pinned);
        //Get the pinned address
        mPixelPtrEmoji = mPixelHandleEmoji.AddrOfPinnedObject();

        mTexCamera = new Texture2D(textureWidth, textureHeight, TextureFormat.ARGB32, false);
        mPixel32Camera = mTexCamera.GetPixels32();
        //Pin pixel32 array
        mPixelHandleCamera = GCHandle.Alloc(mPixel32Camera, GCHandleType.Pinned);
        //Get the pinned address
        mPixelPtrCamera = mPixelHandleCamera.AddrOfPinnedObject();
    }

    public void KeyFrameChangeUpdate()
    {
        string path = poseEditor.timelineFrames[robot.currentFrame].GetComponent<FrameMeta>().filePath;

        if (path.Length > 0)
        {
            Native_OnKeyFrameChange(poseEditor.timelineFrames[robot.currentFrame]
                .GetComponent<FrameMeta>().filePath);

            Logger.Instance.Log(path);
        }
    }

    public void FixUpdate()
    {
        float[] joints = new float[6];
        if (robot.syncMode == 0)
        {
            joints[0] = robot.sliderAngleHead.Value;
            joints[1] = robot.sliderAngleArmRollLeft.Value;
            joints[2] = robot.sliderAngleArmPitchLeft.Value;
            joints[3] = robot.sliderAngleArmRollRight.Value;
            joints[4] = robot.sliderAngleArmPitchRight.Value;
            joints[5] = robot.sliderAngleBody.Value;
        }

        try {
        //Convert Mat to Texture2D
        IntPtr retArrayPtr = Native_OnFixUpdate(mPixelPtrEmoji, mPixelPtrCamera, textureWidth, textureHeight,
            Marshal.UnsafeAddrOfPinnedArrayElement(joints, 0), robot.syncMode == 0);
        float[] retJoints = new float[6];
        Marshal.Copy(retArrayPtr, retJoints, 0, 6);
        if (robot.syncMode == 2)
        {
            robot.targetAngleHead = retJoints[0];
            robot.targetAngleArmRollLeft = retJoints[1];
            robot.targetAngleArmPitchLeft = retJoints[2];
            robot.targetAngleArmRollRight = retJoints[3];
            robot.targetAngleArmPitchRight = retJoints[4];
            robot.targetAngleBody = retJoints[5];
            
            robot.sliderAngleHead.Value = (int) retJoints[0];
            robot.sliderAngleArmRollLeft.Value = (int) retJoints[1];
            robot.sliderAngleArmPitchLeft.Value = (int) retJoints[2];
            robot.sliderAngleArmRollRight.Value = (int) retJoints[3];
            robot.sliderAngleArmPitchRight.Value = (int) retJoints[4];
            robot.sliderAngleBody.Value = (int) retJoints[5];
            
            
            Logger.Instance.Log(retJoints[0]+" "+retJoints[1]+" "+retJoints[2]+" "+
                      retJoints[3]+" "+retJoints[4]+" "+retJoints[5]);
        }

        } catch (EntryPointNotFoundException e) {
        }

        //Update the Texture2D with array updated in C++
        mTexEmoji.SetPixels32(mPixel32Emoji);
        mTexEmoji.Apply();
        mTexCamera.SetPixels32(mPixel32Camera);
        mTexCamera.Apply();
    }


    void OnApplicationQuit()
    {
        mPixelHandleEmoji.Free();
        mPixelHandleCamera.Free();

        try {
            Native_OnExit();
        } catch (EntryPointNotFoundException e) {
            Logger.Instance.LogError("Native_OnExit can't be load");
        }

        Logger.Instance.Log("OnApplicationQuit called");
    }

// #endif
}