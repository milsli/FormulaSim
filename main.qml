import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    // visibility: "FullScreen"
    visible: true
    title: qsTr("Formula 1  Race Simulator")
    id: mainWindow

    signal newRace;
    signal startSignal;
    property int  state: 0

    // states:
    // 0 - start
    // 1 - pauza
    // 2 - nowy wyścig


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
        else
        {
            startButton.text = "Nowy wyścig";
            state = 2;
            mainWindow.newRace();
        }

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

    function resetRace()
    {
        rankId.visible = false;
        for(var i = 1; i < bolidObject.length; ++i)
        {
            bolidObject[i].x = 0;
            bolidObject[i].y = 80 + 42 * i;
        }
        line.x = 70;
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

    Rectangle {
        id: line
        x: 70
        y: 120
        width: 1
        height: 750
        color: 'lightblue'
    }

    Button {
        id: startButton
        text: " S  T  A  R  T "
        x: 890
        y: 950

        onClicked: {
            if(mainWindow.state == 0) {
                text = "  S  T  O  P  ";
                state = 1;
                mainWindow.startSignal();
            } else if (mainWindow.state == 1){
                text = " S  T  A  R  T ";
                state = 0
                mainWindow.startSignal();
            } else if(mainWindow.state == 2) {
                resetRace();
                text = " S  T  A  R  T ";
            }
        }
    }
}
