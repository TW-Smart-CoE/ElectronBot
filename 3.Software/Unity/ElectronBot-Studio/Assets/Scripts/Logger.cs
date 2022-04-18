using UnityEngine;
using System;
using System.Collections.Generic;
class Logger {

    private const int MAX_LOG_COUNT = 50;
    private static readonly Logger logger = new Logger();
    private List<ILoggerListener> _listeners = new List<ILoggerListener>();

    public void AddListener(ILoggerListener listener) {
        _listeners.Add(listener);
    }
    public void RemoveListener(ILoggerListener listener) {
        _listeners.Remove(listener);
    }

    private void RaiseEvent(string s) {
        foreach (var listener in _listeners) {
            try {
                listener.OnLogAdded(this, s);
            } catch (Exception e) {
                Debug.LogError(e.Message);
            }
        }
    }

    public static Logger Instance {
        get {
            // Debug.Log("Logger Instance");
            return logger;
        }
    }

    private List<string> log = new List<string>();

    public int Count {
        get {
            return log.Count;
        }
    }

    public string RichText {
        get {
            /*
            <color=white>info</color>
            <color=yellow>warning</color>
            <color=red>error</color>
            */
            string richText = "";
            foreach (string s in log) {
                if (s.StartsWith("[W]")) {
                    richText += "<color=yellow>" + s + "</color>\n";    
                }
                else if (s.StartsWith("[E]")) {
                    richText += "<color=red>" + s + "</color>\n";    
                }
                else {
                    richText += "<color=white>" + s + "</color>\n";
                }
            }
            return richText;
        }
    }

    public void Log(string s) {
        s = "[I]" + s;
        log.Add(s);
        Debug.Log(s);
        if (log.Count > MAX_LOG_COUNT) {
            log.RemoveAt(0);
        }
        RaiseEvent(s);
    }

    public void LogWarning(string s) {
        s = "[W]" + s;
        log.Add(s);
        Debug.LogWarning(s);
        if (log.Count > MAX_LOG_COUNT) {
            log.RemoveAt(0);
        }
        RaiseEvent(s);
    }
    
    public void LogError(string s) {
        s = "[E]" + s;
        log.Add(s);
        Debug.LogError(s);
        if (log.Count > MAX_LOG_COUNT) {
            log.RemoveAt(0);
        }
        RaiseEvent(s);
    }

}