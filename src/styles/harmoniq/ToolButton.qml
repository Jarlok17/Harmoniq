// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Harmoniq 1.0

T.Button {
    id: control
    property bool isSelected: false
    property url iconSource: ""
    property var buttonManager: null
    property var onClickedAction: null

    width: parent.width
    height: width

    contentItem: Item {
        anchors.fill: parent

        Image {
            anchors.centerIn: parent
            source: iconSource
            sourceSize: Qt.size(control.width * 0.6, control.width * 0.6)
        }
    }

    background: Rectangle {
        color: isSelected ? Qt.lighter(Themes.currentTheme.background, 1.1) : Themes.currentTheme.background
        border.color: control.containsMouse || isSelected ? Themes.currentTheme.accent : "transparent"
        border.width: 2
        radius: 4
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            if (buttonManager) {
                buttonManager.forEach((button) => {
                    if (button !== control && button.hasOwnProperty("isSelected")) {
                        button.isSelected = false;
                    }
                });
            }

            control.isSelected = true;
            console.log("Button clicked: " + iconSource);

            if(onClickedAction)
            {
                onClickedAction();
            }
        }
    }

    
    Component.onCompleted: {
        if (buttonManager && !buttonManager.includes(control)) {
            buttonManager.push(control);
        }
    }
}
