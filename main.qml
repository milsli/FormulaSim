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
    property var crashedBolids: [];
    property int  first: 80

    function newLap(currentLap, laps)
    {
        if(currentLap <= laps)
            lapsText.text = "Okrążenie " + currentLap + "/" + laps;
        rankId.nrOkrazenia = currentLap - 1;
        if(currentLap > 1)
            rankId.visible = true;

        crashedBolids.forEach(e => bolidObject[e].fire = false)
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
        if((first - 66) < x)
        {
            first = x + 66;
            line.x = first;
        }
        bolidObject[nr].x = x;
    }

    function onCrashedCar(nr)
    {
        bolidObject[nr].fire = true;
        crashedBolids.push(nr);
    }

    signal startSignal;
    property bool start: true

    Rectangle {
        id: line
        x: 80
        y: 120


        width: 1
        height: 750

        color: 'lightblue'
    }

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
