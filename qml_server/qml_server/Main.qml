import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    width:550
    height:800
    visible:true
    color:"#ecdcc2" // milk tea color
    title:qsTr("TCP server")

    Item{
        id:root
        objectName:"root"
        property int img_width:0
        property int img_height:0

        Image {
            id:imgItem
            x:80
            y:140
            width:250
            height:250
            visible:true
            source:"qrc:/images/loadingBarV2.png"
        }

        Text {
            id:displayText
            x:80
            y:530
            width:500
            height:50
            visible:true
            font.pixelSize:24
            text:""
        }

        Text {
            id:displayText2
            x:80
            y:590
            width:500
            height:50
            visible:true
            font.pixelSize:24
            text:""
        }

        Text {
            id:displayText3
            x:230
            y:590
            width:500
            height:50
            visible:true
            font.pixelSize:24
            text:""
        }

        Text{
            id:timeDisplayText
            x:120
            y:400
            font.pixelSize:24
            color:"#8600FF" // purple
            text:"mm : ss"
            visible:true
        }

        Text{
            id:timeDisplayText2
            x:230
            y:400
            font.pixelSize:24
            color:"black" // black
            text:"mm : ss"
            visible:true
        }

        Button {
            id:btnItem1
            x:80
            y:700
            width:100
            height:50
            visible:true
            text:"Play"

            onClicked: {
                console.log("Send the string data to Client!")
                myServer.sendSongDataToClient(messageSong);
                myServer.sendArtistDataToClient(messageArtist);
                myServer.sendAlbumDataToClient(messageAlbum);

                displayText.text = messageSong
                displayText2.text = messageArtist
                displayText3.text = "-  " + messageAlbum

                displayText.visible = true
                displayText2.visible = true
                displayText3.visible = true

                timeDisplayText2.text = convertInitTotalTime;
                myServer.sendTotalTimeToClient(TotalTime);
                myTimer.startCountdown(TotalTime);
            }
        }

        Button {
            id:btnItem2
            x:200
            y:700
            width:100
            height:50
            visible:true
            text:"Close"

            onClicked: {
                console.log("Close the program!")
                myServer.sendCloseCommandToClient();
                Qt.quit();
            }
        }

        Button {
            id:btnItem3
            x:320
            y:700
            width:100
            height:50
            visible:true
            text:"Image"

            onClicked: {
                console.log("Send img button click!");
                myServer.sendImageToClient();
            }
        }

        Connections {
            target:myServer
            function onReceivedLocoalImagePath(imagePath){
                imgItem.source = "";
                imgItem.source = Qt.resolvedUrl("file://" + imagePath);
                imgItem.visible = true;
                //                console.log("Display received image success.");
                //                console.log("imagePath: " + Qt.resolvedUrl("file://" + imagePath));
                //                console.log("imgItem(x, y): " + imgItem.x + ", " + imgItem.y);
            }
        }

        Connections {
            target:myTimer
            function onReceivedUpdatedTime(timeString){
                timeDisplayText.text = timeString + "   ";
                //console.log("Updated Time: " + timeString);
            }
        }
    }
}
