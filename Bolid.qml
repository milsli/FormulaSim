import QtQuick 2.15

Item {
    id: bolidImage
    width: 70
    height: 30
    x: 50

    property color hue: "red"
    property string  name: "FER01"
    property bool fire: false

    Rectangle {
        width: 7 * parent.width / 10
        height: 7 * parent.height / 10
        color: parent.hue
        anchors.centerIn: parent

        Text {
            id: bname
            anchors.centerIn: parent
            text: bolidImage.name
        }
    }

    Rectangle {
        width: 2 * parent.width / 10
        height: parent.height
        x: -3
        color: "#d01010"
//        color: parent.hue + "#00a0a0"
    }
    Rectangle {
        width: 1 * parent.width / 10
        height: parent.height
        // anchors.bottomMargin: right
        x: 3 + 8 * parent.width / 10
        color: "#d01010"
    }

    Rectangle {
        height: 1.5 * parent.height / 10
        width: 2 * parent.width / 10
        x: 2 * parent.width / 10
        y: 0
        color: "black"
    }
    Rectangle {
        height: 1.5 * parent.height / 10
        width: 2 * parent.width / 10
        x: 2 * parent.width / 10
        y: parent.height - parent.width / 10
        color: "black"
    }
    Rectangle {
        height: 1.5 * parent.height / 10
        width: 2 * parent.width / 10
        x: 6 * parent.width / 10
        y: 0
        color: "black"
    }
    Rectangle {
        height: 1.5 * parent.height / 10
        width: 2 * parent.width / 10
        x: 6 * parent.width / 10
        y: parent.height - parent.width / 10
        color: "black"
    }

    Image {
        id: explosion
        visible: parent.fire
        x: 15
        y: 5
        width: 40
        height: 40

        source: "explosion.jpg"
    }
}
