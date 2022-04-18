using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class LogButtonBehaviour : MonoBehaviour, ILoggerListener
{

    public GameObject logPanel;

    public Text logTextContent;

    private RectTransform logTextContentRT;

    // private int timeRemain = 1;

    private bool needUpdate = false;

    void Awake() {
        Logger.Instance.AddListener(this);
    }

    void Start() {
        if (logPanel == null) {
            logPanel = GameObject.Find("LogPanel");
        }

        if (logTextContent != null) {
            logTextContentRT = logTextContent.GetComponent<RectTransform>();
        }
    }

    void Update() {
        // if (timeRemain > 0) {
        //     timeRemain--;
        //     if (timeRemain == 0) {
        //         timeRemain = 1;

        //         Logger.Instance.LogWarning("Log Button OnClick");
        //     }
        // }
        if (needUpdate) {
            if (logPanel != null && logTextContent != null && logTextContentRT != null && logPanel.activeInHierarchy) {
                logTextContentRT.sizeDelta = new Vector2(0, Logger.Instance.Count * 32 + 32);
                logTextContent.text = Logger.Instance.RichText;
            }
            needUpdate = false;
        }
    }

    public void OnClick() {
        if (logPanel == null) return;

        logPanel.SetActive(!logPanel.activeInHierarchy);
        if (logPanel.activeInHierarchy) {
            // logTextContentRT.sizeDelta = new Vector2(0, Logger.Instance.Count * 32);
            // logTextContent.text = Logger.Instance.RichText;
            needUpdate = true;
        }
    }

    public void OnLogAdded(object sender, string message) {
        // if (logPanel != null && logTextContent != null && logTextContentRT != null && logPanel.activeInHierarchy) {
        //     logTextContentRT.sizeDelta = new Vector2(0, Logger.Instance.Count * 32);
        //     logTextContent.text = Logger.Instance.RichText;
        // }
        needUpdate = true;
    }
}