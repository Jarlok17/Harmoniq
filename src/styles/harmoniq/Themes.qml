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

    property var currentTheme: darkTheme
}
