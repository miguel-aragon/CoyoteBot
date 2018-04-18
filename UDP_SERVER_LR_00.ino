/*
 * 
 *  Code adapted from: 
 * 
 * 
 * 
 * 
 * 
 */

#include <ESP8266WiFi.h> // Used for the soft AP
#include <WiFiUdp.h> // used for UDP comms.

WiFiUDP Udp;

//--- Soft Ap variables
const char *APssid     = "COYOTEBOT_V1";
const char *APpassword = "";

IPAddress APlocal_IP(192, 168, 4, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);

//***UDP Variables***
unsigned int localUdpPort = 4210;
char incomingPacket[255];
char replyPacket[] = "Hi there! Got the message :-)";

//--- Auxiliar variables
char packetChar[1];
int  startByte;
int  tempi;

//--- PINS
const int PIN_LP = 12; // PWM Left
const int PIN_RP = 14; // PWM Right
const int PIN_L0 =  0; // Direction Left
const int PIN_L1 =  2; // Direction Left
const int PIN_R0 =  4; // Direction Right
const int PIN_R1 =  5; // Direction Right

//=============================================
//
//=============================================
 void define_pins(){
   pinMode(PIN_LP, OUTPUT);
   pinMode(PIN_RP, OUTPUT);
   pinMode(PIN_L0, OUTPUT);
   pinMode(PIN_L1, OUTPUT);
   pinMode(PIN_R0, OUTPUT);
   pinMode(PIN_R1, OUTPUT);
 }

//=============================================
//
//=============================================
void startup_motors(){
   digitalWrite(PIN_L1,LOW);
   digitalWrite(PIN_L0,HIGH);
   analogWrite(PIN_LP, 255);
   delay(1000);
   digitalWrite(PIN_L1,LOW);
   digitalWrite(PIN_L0,LOW);     
      
   digitalWrite(PIN_R1,LOW);
   digitalWrite(PIN_R0,HIGH);
   digitalWrite(PIN_RP,LOW);
   analogWrite(PIN_RP, 255);
   delay(1000);
   digitalWrite(PIN_R0,LOW);
   digitalWrite(PIN_R1,LOW);
 }

//=============================================
//
//=============================================
void setup() {


 Serial.begin(115200); //fire up the serial port
 
 WiFi.mode(WIFI_AP_STA);
 
 Serial.println("ESP8266 AP & Station & UDP System test");
 // Configure the Soft Access Point. Somewhat verbosely... (for completeness sake)
 Serial.print("Soft-AP configuration ... ");
 Serial.println(WiFi.softAPConfig(APlocal_IP, APgateway, APsubnet) ? "OK" : "Failed!"); // configure network
 Serial.print("Setting soft-AP ... ");
 Serial.println(WiFi.softAP(APssid, APpassword) ? "OK" : "Failed!"); // Setup the Access Point
 Serial.print("Soft-AP IP address = ");
 Serial.println(WiFi.softAPIP()); // Confirm AP IP address
 
 // Setup the UDP port
 Serial.println("begin UDP port");
 Udp.begin(localUdpPort);
 Serial.print("local UDP port: ");
 Serial.println(localUdpPort );


 //--- Pins definition
 define_pins();
 
 //--- Small test just for show-off
 startup_motors();
 
}

//=============================================
//
//=============================================
void loop() {
 
 int packetSize = Udp.parsePacket();
 if (packetSize) {
   // receive incoming UDP packets
   Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

   //--- Read header of packet
   startByte = Udp.read();

   //--- Left motor
   if ( startByte == 'L') {//check if the byte is the start of a package
    packetChar[0] = Udp.read();
    tempi = atoi (packetChar);

    //--- Define direction
    if (tempi < 5){
    digitalWrite(PIN_L0,LOW);
    digitalWrite(PIN_L1,HIGH);
   } else {
    digitalWrite(PIN_L0,HIGH);
    digitalWrite(PIN_L1,LOW);
   }
    Serial.println(tempi, DEC);
    analogWrite(PIN_LP, (abs(tempi-4)*226));
   }

   //--- Right motor
   if ( startByte == 'R') {//check if the byte is the start of a package
    packetChar[0] = Udp.read();
    tempi = atoi (packetChar);

    //--- Define direction
    if (tempi < 5){
    digitalWrite(PIN_R0,LOW);
    digitalWrite(PIN_R1,HIGH);
   } else {
    digitalWrite(PIN_R0,HIGH);
    digitalWrite(PIN_R1,LOW);
   }    
    Serial.println(tempi, DEC);
    analogWrite(PIN_RP, (abs(tempi-4)*226));
   }
 
 } //  if (packetSize)


} // loop()









