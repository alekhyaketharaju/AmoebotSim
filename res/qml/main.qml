import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import VisItem 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: "AmoebotSim"

    signal start()
    signal stop()
    signal round()
    signal back()

    signal executeCommand(string cmd)
    function log(msg, isError)
    {
        commandField.visible = false
        buttonRow.forceActiveFocus()

        resultField.text = msg
        resultField.showsError = isError;
        resultField.opacity = 1
        resultField.visible = true
        resultFieldFade.running = true
    }

    function setLabelStart()
    {
        startStopButton.text = "start"
    }

    function setLabelStop()
    {
        startStopButton.text = "stop"
    }

    VisItem {
        id: vis
        anchors.fill: parent
    }

    A_ResultTextField {
        id: resultField
        visible: false
        opacity: 0
        anchors.margins: 10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: window.width - 200

        SequentialAnimation {
            id: resultFieldFade
            running: false

            PauseAnimation {
                duration: 5000
            }

            NumberAnimation {
                target: resultField
                property: "opacity"
                to: 0
                duration: 1000
            }
        }
    }

    Row {
        id: buttonRow
        spacing: 10
        anchors.margins: 10
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        A_TextField {
            id: commandField
            visible: false
            width: window.width - 200

            focus: true
            Keys.onPressed: {
                if(event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                    executeCommand(text)
                    visible = false
                    text = ""
                    buttonRow.forceActiveFocus()
                    event.accepted = true
                } else if(event.key === Qt.Key_Escape) {
                    visible = false
                    text = ""
                    buttonRow.forceActiveFocus()
                    event.accepted = true
                }
            }
        }

        focus: true
        Keys.onPressed: {
            if(event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                resultField.visible = false
                resultFieldFade.running = false
                commandField.visible = true
                commandField.forceActiveFocus()
                event.accepted = true
            }
        }

        A_Button {
            id: startStopButton
            text: "start"

            onClicked: {             
                if(text == "start") {
                    start()
                    text = "stop"
                } else {
                    stop()
                    text = "start"
                }
            }
        }

        A_Button {
            id: roundButton
            text: "round"
            onClicked: {
                round()
            }
        }

//        A_Button {
//            id: backButton
//            text: "back"
//            onClicked: {
//                back()
//            }
//        }
    }
}
