using System.Reflection;
using UnityEditor;

public static class UsefulShortcuts
{
    [MenuItem("Tools/Clear Console %#x")] // CMD + SHIFT + X
    static void ClearConsole()
    {
        var assembly = Assembly.GetAssembly(typeof(SceneView));
        var type = assembly.GetType("UnityEditor.LogEntries");
        var method = type.GetMethod("Clear");
        method.Invoke(new object(), null);
    }
}
