import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property int nrOkrazenia: 1
    x: 1650
    y: 150
    height: 600

    property variant standings: []
    property variant crashed: []

    TextField {
        text: "Ranking po " + nrOkrazenia + " okrążeniu"
        y: 20
    }

    Column {
        y: 70
        Column {
            Repeater {
                model: standings

                Text {
                    required property int index
                    text: index + 1 + "  " + standings[index]
                 }
            }
        }
        Column {
            Repeater {
                model: crashed

                Text {
                    required property int index
                    text:  "  " + crashed[index].name + "  DFN"
                 }
            }
        }
    }
}
