import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

// import QtQuick.Templates 2.15


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
        id: rankId
        standings: supervisor.rankingTable
        crashed: supervisor.crashedCars
    }

    property var bolidObject: [];

    function newLap(currentLap, laps)
    {
        lapsText.text = "Okrążenie " + currentLap + "/" + laps;
        rankId.nrOkrazenia = currentLap;
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

    Button {
        text: " S  T  A  R  T "
        onClicked: mainWindow.startSignal();
        x: 890
        y: 950

    }
}
