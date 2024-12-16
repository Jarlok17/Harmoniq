// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import Harmoniq 1.0

MenuBar {
    id: control

    delegate: MenuBarItem {
        id: menuBarItem

        contentItem: Text {
            text: menuBarItem.text
            font.pixelSize: Themes.currentTheme.fontSize
            opacity: enabled ? 1.0 : 0.3
            color: Themes.currentTheme.text
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 40
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            color: menuBarItem.highlighted ? Themes.currentTheme.accent : "transparent"
        }
    }
    
    /*background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40

        Rectangle {
            color: "#000000"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }*/
}
