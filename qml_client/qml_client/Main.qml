import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15


Window {
    width:880
    height:500
    visible:true
    color:"#800080" // purple
    title:qsTr("TCP client")

    Text {
        id:receivedText
        x:12
        y:350
        font.pixelSize:24
        color:"#ffff00" // yellow
        text:"Loding Songs..."
    }

    Text {
        id:receivedText2
        x:12
        y:400
        font.pixelSize:24
        color:"#ffff00" // yellow
        text:"Artis name"
    }

    Text {
        id:receivedText3
        x:162
        y:400
        font.pixelSize:24
        color:"#ffff00" // yellow
        text:"Album name"
    }

    Text{
        id:timeDisplayText
        x:90
        y:290
        font.pixelSize:24
        color:"#53FF53" // light green
        text:"mm : ss"
        visible:true
    }

    Text{
        id:timeDisplayText2
        x:190
        y:290
        font.pixelSize:24
        color:"#FF8800" // orange
        text:"mm : ss"
        visible:true
    }

    Image {
        id:receivedImage
        x:80
        y:80
        width:200
        height:200
        source:"qrc:/images/loadingBarV3.png"
        visible:true
    }

    Connections {
        target: myClient
        function onReceivedSongData(data) {
            receivedText.text = data;
//            console.log(receivedText.text);
//            console.log("receivedText(x, y): " + receivedText.x + ", " + receivedText.y);
        }
        function onReceivedArtistData(data){
            receivedText2.text = data;
//            console.log(receivedText2.text);
//            console.log("ArtistData(x, y): " + receivedText2.x + ", " + receivedText2.y);
        }
        function onReceivedAlbumData(data){
            receivedText3.text = "-  " + data;
//            console.log(receivedText3.text);
//            console.log("AlbumData(x, y): " + receivedText3.x + ", " + receivedText3.y);
        }
        function onReceivedImageDataPath(imagePath){
            receivedImage.source = "";
            receivedImage.source = Qt.resolvedUrl("file://" + imagePath);
            receivedImage.visible = true;
//            receivedText4.text = "123 bytes";
//            receivedText4.visible = true;
//            console.log("receivedText4.text: " + receivedText4.text);
//            console.log("Display received image success.");
//            console.log("imagePath: " + Qt.resolvedUrl("file://" + imagePath));
//            console.log("receivedImage(x, y): " + receivedImage.x + ", " + receivedImage.y);
        }
        function onReceivedInitTotalTime(stringTotalTime){
            timeDisplayText2.text = stringTotalTime;
            //console.log("Total Time: " + stringTotalTime);
        }
    }

    Connections {
        target: myTimerObject
        function onReceivedUpdatedTime(timeString){
            timeDisplayText.text = timeString + "   ";
            //console.log("Updated Time: " + timeString);
        }
    }
}
