// 
// Simple controller for esp8266 from android phone over UDP.
//
// Written by Miguel Aragon, 2018.
// 
//
// Notes: 
// Multitouch code from http://www.akeric.com/blog/?p=1411
// Eric Pavey - AkEric.com - 2010-12-29
//   Example showing simple multi-touch detection in 'Processing - Android'.
//   My Samsung Captivate (Galaxy S) can track 5 touch-points.
//   Updated to work with Processing's surfaceTouchEvent instead of
//   Android's dispatchTouchEvent.

// Should point out that API Level 9 has MotionEvent.PointerCoords class that
// expose some cool functionality, but I'm only on Level 7.

import android.view.MotionEvent;
import hypermedia.net.*;

UDP udp;  // define the UDP object

//--- UDP client
String IP = "192.168.4.1";    // the remote IP address
int port  = 4210;        // the destination port

//--- Cursor
float cursor_L;
float cursor_R;

String[] fontList;
PFont androidFont;



//-----------------------------------------------
//
//-----------------------------------------------
void setup() {
  size(displayWidth, displayHeight);
  // Fix the orientation so the sketch won't reset when rotated.
  orientation(LANDSCAPE);
  stroke(255);
  //smooth();
  // Setup Fonts:
  fontList = PFont.list();
  androidFont = createFont(fontList[0], 16, true);
  textFont(androidFont);
  textAlign(LEFT);
  
  // create a new datagram connection on port 6000
  // and wait for incomming message
  udp = new UDP( this, 6000 );
  udp.listen( true );
}

//-----------------------------------------------
//
//-----------------------------------------------
void draw() {
  background(0);
  
  line(20, displayHeight/2, displayWidth-20, displayHeight/2);
  infoCircle(100,              cursor_L, 50);
  send_packet("L", int(cursor_L/float(displayHeight)*10));
  
  infoCircle(displayWidth-100, cursor_R, 50);
  send_packet("R", int(cursor_R/float(displayHeight)*10));
  
}

//-----------------------------------------------
//
//-----------------------------------------------
void infoCircle(float x, float y, float siz) {
  float diameter = siz;
  noFill();
  ellipse(x, y, diameter, diameter);
  line(x-diameter*2, y, x+diameter*2,y);
}

//-----------------------------------------------
//
//-----------------------------------------------
void send_packet(String motor, int value) {
       //-- formats the message for Pd
    String message = motor + str(value);
    //-- send the message
    udp.send( message, IP, port );
}

//-----------------------------------------------
// Override Processing's surfaceTouchEvent, which will intercept all
// screen touch events.  This code only runs when the screen is touched.
//-----------------------------------------------
public boolean surfaceTouchEvent(MotionEvent me) {
  // Number of places on the screen being touched:
  int numPointers = me.getPointerCount();
  for(int i=0; i < numPointers; i++) {
    int pointerId = me.getPointerId(i);
    float x = me.getX(i);
    float y = me.getY(i);
    float siz = me.getSize(i);
    
    if (x < displayWidth/2) cursor_L = y;
    if (x > displayWidth/2) cursor_R = y;

  }
  // If you want the variables for motionX/motionY, mouseX/mouseY etc.
  // to work properly, you'll need to call super.surfaceTouchEvent().
  return super.surfaceTouchEvent(me);
}

/**
 * To perform any action on datagram reception, you need to implement this 
 * handler in your code. This method will be automatically called by the UDP 
 * object each time he receive a nonnull message.
 * By default, this method have just one argument (the received message as 
 * byte[] array), but in addition, two arguments (representing in order the 
 * sender IP address and his port) can be set like below.
 */
void receive( byte[] data, String ip, int port ) {    // <-- extended handler
  
  // get the "real" message =
  // forget the ";\n" at the end <-- !!! only for a communication with Pd !!!
  data = subset(data, 0, data.length-2);
  String message = new String( data );
  
  // print the result
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
}
