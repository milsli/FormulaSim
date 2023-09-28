import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    visibility: "FullScreen"
    visible: true
    title: qsTr("Formula 1  Race Simulator")
    id: mainWindow

    TextField {
        id: lapsText
        x: 800
        y: 40
        text: ""
    }

    Ranking {
        visible: false
        id: rankId
        standings: supervisor.rankingTable
        crashed: supervisor.crashedCars
    }

    property var bolidObject: [];

    function newLap(currentLap, laps)
    {
        if(currentLap <= laps)
            lapsText.text = "Okrążenie " + currentLap + "/" + laps;
        rankId.nrOkrazenia = currentLap - 1;
        if(currentLap > 1)
            rankId.visible = true;


    }

    function createBolid(nr, name, hue)
    {
        var bolid;
        bolid = Qt.createComponent("Bolid.qml");

        if(bolid.status === Component.Ready)
        {
            bolidObject[nr] = bolid.createObject(mainWindow, {"name": name, "x": 0, "y": 80 + 42 * nr, "hue": hue});
        }
    }

    function moveBolid(nr, x)
    {
        bolidObject[nr].x = x;                
    }

    signal startSignal;
    property bool start: true

    Button {
        text: " S  T  A  R  T "
        x: 890
        y: 950

        onClicked: {
            if(start == true) {
                text = "  S  T  O  P  ";
                start = false;
            } else {
                text = " S  T  A  R  T ";
                start = true
            }

            mainWindow.startSignal();
        }
    }
}
