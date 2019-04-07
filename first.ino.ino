#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
int wifiPinStatus[8][2] = {{5, LOW}, {4, LOW}, {0, LOW}, {2, LOW}, {14, LOW}, {12, LOW}, {13, LOW}, {15, LOW}};
int getParams() {
  if (server.arg("switch_num") == "") {
    return 0;
  }
  else {
    String a = server.arg("switch_num");
    return a.toInt();
  }
}
String getjson(String error,String st){
    String json = "{\"status\":" + st + ",\"result\":{\"error\":\"" + error + "\",\"switch_status\":[" + wifiPinStatus[0][1] + ',' + wifiPinStatus[1][1] + ',' + wifiPinStatus[2][1] + ',' + wifiPinStatus[3][1] + ',' + wifiPinStatus[4][1] + ',' + wifiPinStatus[5][1] + ',' + wifiPinStatus[6][1] + ',' + wifiPinStatus[7][1] + "]}}";
    Serial.println(json);
    return json;
}
void handleRoot(){
  String error = "",st = "200";
  String json = "{\"status\":" + st + ",\"result\":{\"error\":\"" + error + "\",\"switch_status\":[" + wifiPinStatus[0][1] + ',' + wifiPinStatus[1][1] + ',' + wifiPinStatus[2][1] + ',' + wifiPinStatus[3][1] + ',' + wifiPinStatus[4][1] + ',' + wifiPinStatus[5][1] + ',' + wifiPinStatus[6][1] + ',' + wifiPinStatus[7][1] + "]}}";
  Serial.println(json);
  server.send(200, "application/json", json);
}
void handleOn() {
  int num = getParams() - 1;
  String error = "", st = "200";
  if(num<0||num>7){
    if(num == -1)
      error = "Invalid Key in Key-Value pair";
    else
      error = "Incorrect pin value entered";
  }
  digitalWrite(wifiPinStatus[num][0], HIGH);
  wifiPinStatus[num][1] = HIGH;
  server.send(200, "application/json", getjson(error,st));
}
void handleOff() {
  int num = getParams() - 1;
  String error = "",st = "200";
  if(num<0||num>7){
    if(num == -1)
      error = "Invalid Key in Key-Value pair";
    else
      error = "Incorrect pin value entered";
    st = "401";
  }
  digitalWrite(wifiPinStatus[num][0], LOW);
  wifiPinStatus[num][1] = LOW;
  server.send(200, "application/json", getjson(error,st));
}
void connectToWifi(){
  WiFi.begin("SSID","PASSWORD");
  Serial.println("Connecting to \"SSID\"\n...");
  if(WiFi.waitForConnectResult() == WL_CONNECTED){
    //again not required since there is no request to send response to.
    //server.send(401,"text/plain","Couldn't connect to WiFi");
    Serial.println("");
    Serial.println("Successfully connected");
    Serial.println("Server started");
     //Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
  }
  else
    Serial.println("Failed to connect");
}
void setup() {
  WiFi.mode(WIFI_AP_STA);
  Serial.begin(115200);
  connectToWifi();
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
