using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

public interface ILogger
{
    void Debug(object message);

    void Info(object message);

    void Warn(object message);

    void Error(object message);
}

public sealed class LogManager : IDisposable, ILogger
{
    public static readonly LogManager Instance = new LogManager();
    private UdpClient udpClient = null;
    private IPEndPoint remoteEP = null;

    private LogManager()
    {
        if (PlatformUtils.IsDesktop() || PlatformUtils.IsUnityEditor())
        {
            udpClient = new UdpClient();
            remoteEP = new IPEndPoint(IPAddress.Loopback, 11000);
        }
    }

    public void Dispose()
    {
        if (PlatformUtils.IsDesktop() || PlatformUtils.IsUnityEditor())
        {
            udpClient.Dispose();
        }
    }

    private void UdpLog(object message)
    {
        if (PlatformUtils.IsDesktop() || PlatformUtils.IsUnityEditor())
        {
            var bytes = Encoding.ASCII.GetBytes(message.ToString());
            udpClient.Send(bytes, bytes.Length, remoteEP);
        }
    }

    public void Debug(object message)
    {
        UnityEngine.Debug.Log(message);
        UdpLog("[DEBUG] " + message);
    }

    public void Info(object message)
    {
        UnityEngine.Debug.Log(message);
        UdpLog("[INFO] " + message);
    }

    public void Warn(object message)
    {
        UnityEngine.Debug.LogWarning(message);
        UdpLog("[WARN] " + message);
    }

    public void Error(object message)
    {
        UnityEngine.Debug.LogError(message);
        UdpLog("[ERROR] " + message);
    }
}
