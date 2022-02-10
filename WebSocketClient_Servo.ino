/*
 * WebSocketClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient.h>

#include <Hash.h>

#include <Servo.h>

Servo myservo;

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define USE_SERIAL Serial

  char sensor[3];
  char dir[3];
  int value[3];
  String s_name;
  String d;
  String v;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t len) {

	switch(type) {
		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED: {
			USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			//webSocket.sendTXT("Connected");
     String datain = "{\"0\":\"ESP\",\"1\":\"0\"}";
       webSocket.sendTXT(datain);
		}
			break;
		case WStype_TEXT:
			USE_SERIAL.printf("[WSc] get text: %s\n", payload);
  
			// send message to server
			// webSocket.sendTXT("message here");
       Serial.print(*payload);
     Serial.print(len);
     Serial.print((const char*)payload);
//     pay_str=(const char*)payload;
//     pay_len=len;
     for (int i=1; i<4;i++){
      sensor[i-1]=(char)payload[i];
      s_name=s_name+sensor[i-1];
      Serial.print("sensor inside event\n");
      Serial.print(sensor[i-1]);
   //  Serial.print((char)payload[i]);
     }
     Serial.print("s inside event \n");
     Serial.print(s_name);
     for (int i=4; i<7;i++){
      dir[i-4]=(char)payload[i];
    //  d=d+dir[i-4];
    //Serial.print((char)payload[i]);
    ;
     }
     for (int i=7; i<10;i++){
      //value[i-7]=(char)payload[i];
     // v=v+value[i-8];
     //Serial.print((char)payload[i]);
     }

     //Serial.print("string cumulated\n");
     //Serial.print(s_name);
     Serial.print("sensor:\n");
     Serial.print(sensor);
			break;
      /*
		case WStype_BIN:
			USE_SERIAL.printf("[WSc] get binary length: %u\n", len);
			hexdump(payload, len);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
        case WStype_PING:
            // pong will be send automatically
            USE_SERIAL.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            USE_SERIAL.printf("[WSc] get pong\n");
            break;*/
    }

}

void setup() {
	// USE_SERIAL.begin(921600);
	USE_SERIAL.begin(115200);

 myservo.attach(D2,500,2400);

	//Serial.setDebugOutput(true);
	USE_SERIAL.setDebugOutput(true);

	USE_SERIAL.println();
	USE_SERIAL.println();
	USE_SERIAL.println();

	for(uint8_t t = 4; t > 0; t--) {
		USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}

	WiFiMulti.addAP(SSID,PASS);
  //WiFiMulti.addAP(process.env.SSID, process.env.PASS);

	//WiFi.disconnect();
	while(WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}

	// server address, port and URL
	webSocket.begin("192.168.1.8", 3000, "/");

	// event handler
	webSocket.onEvent(webSocketEvent);

	// use HTTP Basic Authorization this is optional remove if not needed
	//webSocket.setAuthorization("user", "Password");

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(5000);
  
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);

}

void loop() {
  int pos;
  int ang;
	char scomp[]={'M','O','T','\0'};
	webSocket.loop();

  if(strcmp(sensor,scomp)==0){
   // delay(1000);
    Serial.print("inside char comp");
    ang=atoi(dir);
    sensor[1]='-1';
    s_name="";
    

  Serial.print("angle\n");
  Serial.print(ang);
  
  
  

  for (pos = 0; pos <= ang; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.print("\n");
    Serial.print(myservo.readMicroseconds());
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = ang; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}
}
