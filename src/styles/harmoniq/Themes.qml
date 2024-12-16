pragma Singleton
import QtQuick 2.15

QtObject {
    property var darkTheme: {
        "background": "#2E2E2E",
        "foreground": "#FFFFFF",
        "primary": "#1E88E5",
        "accent": "#F06292",
        "text": "#E0E0E0",
        "fontSize": 14
    }

    property var lightTheme: {
        "background": "#F5F5F5",
        "foreground": "#000000",
        "primary": "#2196F3",
        "accent": "#E91E63",
        "text": "#212121",
        "fontSize": 14
    }

    property var currentTheme: darkTheme
}
