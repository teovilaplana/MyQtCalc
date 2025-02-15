pragma ComponentBehavior: Bound

import QtQuick
//import "calculator.h" as CalcEngine
import QtQuick.Layouts

Item {
    id: controller
    implicitWidth: isPortraitMode ? portraitModeWidth : landscapeModeWidth
    implicitHeight: mainGrid.height

    readonly property color qtGreenColor: "#2CDE85"
    readonly property color backspaceRedColor: "#DE2C2C"
    readonly property int spacing: 5

    property bool isPortraitMode: root.isPortraitMode
    property int portraitModeWidth: mainGrid.width
    property int landscapeModeWidth: scientificGrid.width + mainGrid.width

    function updateDimmed() {
        for (let i = 0; i < mainGrid.children.length; i++) {

        }
    }

    component DigitButton: CalcButton {
        onReleased: {
            root.digitPressed(text)
            updateDimmed()
        }
    }

    component OperatorButton: CalcButton {
        onReleased: {
            root.operatorPressed(text)
            updateDimmed()
        }
        textColor: controller.qtGreenColor
        implicitWidth: 48
        dimmable: true
    }

    Component.onCompleted:  updateDimmed()

    Rectangle {
        id: keyPad
        anchors.fill: parent
        radius: 8
        color: "transparent"

        RowLayout {
            spacing: controller.spacing

            GridLayout {
                id: scientificGrid
                columns: 3
                columnSpacing: controller.spacing
                rowSpacing: controller.spacing
                visible: !isPortraitMode

                OperatorButton { text: "x²" }
                OperatorButton { text: "⅟x" }
                OperatorButton { text: "√" }
                OperatorButton { text: "x³" }
                OperatorButton { text: "sin" }
                OperatorButton { text: "|x|" }
                OperatorButton { text: "log" }
                OperatorButton { text: "cos" }
                DigitButton {
                    text: "e"
                    dimmable: true
                    implicitWidth: 48
                }
                OperatorButton { text: "ln" }
                OperatorButton { text: "tan" }
                DigitButton {
                    text: "π"
                    dimmable: true
                    implicitWidth: 48
                }
            }

            GridLayout {
                id: mainGrid
                columns: 5
                columnSpacing: controller.spacing
                rowSpacing: controller.spacing

                BackSpace {}
                DigitButton { text: "7" }
                DigitButton { text: "8" }
                DigitButton { text: "9" }
                OperatorButton {
                    text: "÷"
                    implicitWidth: 38
                }

                OperatorButton {
                    text: "AC"
                    textColor: controller.backspaceRedColor
                    accentColor: controller.backspaceRedColor
                }
                DigitButton { text: "4" }
                DigitButton { text: "5" }
                DigitButton { text: "6" }
                OperatorButton {
                    text: "×"
                    implicitWidth: 38
                }

                OperatorButton {
                    text: "="
                    implicitHeight: 81
                    Layout.rowSpan: 2
                }

                DigitButton { text: "1" }
                DigitButton { text: "2" }
                DigitButton { text: "3" }
                OperatorButton {
                    text: "-"
                    implicitWidth: 38
                }

                OperatorButton {
                    text: "±"
                    implicitWidth: 38
                }
                DigitButton { text: "0" }
                DigitButton {
                    text: "."
                    dimmable: true
                }
                OperatorButton {
                    text: "+"
                    implicitWidth: 38
                }
            }
        } // RowLayout
    }

}
