' Show a simple error message box when the script runs
Option Explicit

Dim result, sh
Set sh = CreateObject("WScript.Shell")

Do
    result = MsgBox("Error: License file encrypted.", vbExclamation + vbRetryCancel, "Error")
    If result = vbRetry Then
        ' Show a retrying message that auto-closes after 2 seconds
        sh.Popup "Retrying...", 2, "Please wait", vbInformation
    End If
Loop While result = vbRetry

Set sh = Nothing
WScript.Quit
