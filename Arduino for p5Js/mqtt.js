 new_element=document.createElement("script");
 new_element.setAttribute("type","text/javascript");
 new_element.setAttribute("src","https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.min.js");// 在这里引入了a.js
 document.body.appendChild(new_element);


client = new Paho.MQTT.Client("broker.hivemq.com", Number(8000),"nijiuhaola");  
client.connect({onSuccess:onConnect});   
var temperature=0;
var last_temperature=0;
var isStart=0;
var isUpdate;
var colorFadeFlag=0;

function onConnect() {  
    	console.log("onConnected");  
    	client.subscribe("jaco/breathinglight");
	isUpdate=0;
}  

    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

function onConnectionLost(responseObject) {
        if (responseObject.errorCode !== 0) {
               console.log("onConnectionLost:"+responseObject.errorMessage);
               console.log("connection lost");
		isStart=0;
		client.connect();   
             }
        }


function onMessageArrived(message) {
	console.log("message arrived: "+message.payloadString);
	temperature=parseFloat(message.payloadString);
	
	if (!isStart)  {
		isStart=1;
		last_temperature=temperature;
		return 0;
	}
	
	isUpdate=1;   //second time receive message
	

	if (last_temperature!=temperature) {
		colorFadeFlag =1;
		last_temperature=temperature;
	}
}
	



