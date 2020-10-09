const cmdName=1
const cmdMSG=2

var ws;
var name;
function websocketSupported(){
    return true;
}
function setupWebsocket(){
    url="ws://10.42.0.10:5555/chat";
   ws = new WebSocket(url);
   ws.onopen= function(event) {
       wsOpen(event);
   }
   ws.onmessage=function(event) {
       wsRecv(event);
   }
   ws.onerror=function(event) {
       wsError(event);
   }
   ws.onclose=function(event) {
       wsClose(event);
   }
};

function wsOpen(event) {   
    console.log("Socket Opened");
    wsSend(cmdName,name,"");
    enableChatWindow(); 
}

function wsSend(ID,name,data) {
    var jsonobj={"ID":ID,"Name":name,"data":data};
    jsonString=JSON.stringify(jsonobj)
    console.log(jsonString);
    ws.send(jsonData);
}

function wsRecv(event) {    
    console.log("ws Data Recived");
    jsonString=event.data;
    jsonObj=JSON.parse(jsonString);

    if (jsonObj.ID == cmdMSG){
        handleIncomingMsg(jsonObj.Name,jsonObj.data);
    }
}
function wsError(event) {    
    console.log("ws Error"); 
}
function wsClose(event) {    
    console.log("ws Closed"); 
}

function handleIncomingMsg(who,msg){
    console.log("from:"+who+" Msg:"+msg);
}

function enableChatWindow() {
    
}

$(document).ready(function(){

    $("#name_input").click(function(event){
        $(this).removeClass("is-invalid");
    })

    $("#join_button").click(function(){
        if($("#name_input").val()!=""){
            $("#name_input").addClass("is-valid");
            name=$("#name_input").val();
            if(websocketSupported())
                setupWebsocket();

        }else{
            $("#name_input").focus();
            $("#name_input").addClass("is-invalid");
        }
    });
});