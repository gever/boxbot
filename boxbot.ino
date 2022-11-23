/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include "util.h"

#define SDEBUG(label, x) {Serial.print(label); Serial.println(x);}
#define ABS(x) ((x<0) ? (-x) : (x))

// the gui is served as static "files" from memory
extern char *index_html;
extern char *style_css;
extern char *script_js;

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "boxbot01";
const char *password = "yourPassword";

WebServer server(80);

const bool FWD = true;
const bool BWD = false;

#define DW(p, v) digitalWrite(p, v)
#define SET_BITS(a, b, c, d) { DW(pin1, a); DW(pin2, b); DW(pin3, c); DW(pin4, d); }

class MarsStepper {
  private:
    bool enabled;
    bool dir;
    unsigned short pin1, pin2, pin3, pin4;
    unsigned int step_counter;
    unsigned int divisor;
  public:
    unsigned int current_step;
    MarsStepper(int motor_pin_1, int motor_pin_2,
                int motor_pin_3, int motor_pin_4) {
      pin1 = motor_pin_1;
      pin2 = motor_pin_2;
      pin3 = motor_pin_3;
      pin4 = motor_pin_4;

      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      pinMode(pin3, OUTPUT);
      pinMode(pin4, OUTPUT);

      current_step = 0; // actual rotation/phase of motor
      step_counter = 0; // keeping track of how often step() function is called
      divisor = 1; // how often step() calls should be heeded

      enabled = false;
      dir = FWD;
    }
    void set_direction(bool d) {
      // SDEBUG("set_direction = ", d);
      dir = d;
    }
    void enable() {
      enabled = true;
    }
    void disable() {
      enabled = false;
      SET_BITS(0, 0, 0, 0);
    }
    void set_divisor( unsigned int v ) {
      divisor = v;
    }
    void step() {
      // always increment the counter
      step_counter++;

      // don't do anything if we're not running
      if (!enabled) return;

      // don't do anything if it's not our turn yet
      if (step_counter % divisor)
        return;

      // step! (activate the next set of stators)
      switch ( current_step % 8 ) {
        case 0: SET_BITS(1, 0, 0, 0); break;
        case 1: SET_BITS(1, 1, 0, 0); break;
        case 2: SET_BITS(0, 1, 0, 0); break;
        case 3: SET_BITS(0, 1, 1, 0); break;
        case 4: SET_BITS(0, 0, 1, 0); break;
        case 5: SET_BITS(0, 0, 1, 1); break;
        case 6: SET_BITS(0, 0, 0, 1); break;
        case 7: SET_BITS(1, 0, 0, 1); break;
      }
      current_step += (dir ? 1 : -1); // FWD=true, BWD=false
    }
};


// create the motors
MarsStepper m1(13, 14, 27, 26);
MarsStepper m2(18, 19, 21, 22);

#define ROT_STEPS  4000
#define ROT_UNIT (ROT_STEPS/16)

int step_count = 0; // for the current motion, for all active motors

/*
   set up a move command
*/
void setup_move(bool dir, int dist) {
  // SDEBUG("setup_move\ndir = ", dir);
  // SDEBUG("dist = ", dist);
  step_count = dist * ROT_UNIT;
  if (dir) {
    // SDEBUG("FWD:", dir);
    m1.set_direction(0);
    m2.set_direction(1);
  } else {
    // SDEBUG("BWD:", dir);
    m1.set_direction(1);
    m2.set_direction(0);
  }
  m1.enable();
  m2.enable();
}

/*
   set up a turn command
*/
void setup_turn(int dir, int dist) {
  step_count = (dist * ROT_UNIT) / 4;
  if (dir) {
    m1.set_direction(1);
    m2.set_direction(1);
  } else {
    m1.set_direction(0);
    m2.set_direction(0);
  }
  m1.enable();
  m2.enable();
}

void step_the_motors() {
  if (step_count) {
    step_count--;
    
    m1.step();
    m2.step();
    
    if (step_count == 0) {
      // save power, turn off the motors when not moving
      m1.disable();
      m2.disable();
    }
  }
}

// hardware clock to step the motors at a good rate
hw_timer_t *step_timer = NULL;

void IRAM_ATTR onTimer(){
  step_the_motors();
}

/*
 * Handle requests for static page content
 */
void handleLandingPage() {
  server.send(200, "text/html", index_html);
}
void handleScriptJs() {
  server.send(200, "text/javascript", script_js);
}
void handleStyleCss() {
  server.send(200, "text/css", style_css);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

// CONSIDER: right now you can interrupt the active movement by
//           sending a new movement command

void handleMove() {
  if (server.args()) {
    // SDEBUG("move: args = ", server.args());
    // SDEBUG("move: arg = ", server.arg(0));
    int v = server.arg(0).toInt();  // negative for backwards movement
    // SDEBUG("move: v = ", v);
    setup_move( v < 0 ? BWD : FWD, ABS(v) );
  }
  server.send(200, "application/json", "{status:'ACK'}");
}

void handleTurn() {
  if (server.args()) {
    int v = server.arg(0).toInt();
    setup_turn( v < 0 ? 0 : 1, ABS(v) ); // negative for left turns
  }
  // Serial.print("step_count="); Serial.println(step_count);
  server.send(200, "application/json", "{status:'ACK'}");
}

void handleStop() {
  m1.disable();
  m2.disable();
  step_count = 0;
  server.send(200, "application/json", "{status:'ACK'}");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  Serial.println("Configuring access point...");
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleLandingPage);
  server.on("/move", handleMove);
  server.on("/turn", handleTurn);
  server.on("/stop", handleStop);
  server.on("/index.html", handleLandingPage);
  server.on("/style.css", handleStyleCss);
  server.on("/script.js", handleScriptJs);
  server.onNotFound(handleNotFound);
  
  server.begin();

  // set up the motor step timer
  step_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(step_timer, &onTimer, true);
  timerAlarmWrite(step_timer, 850 , true);  // second parameter is the delay time, smaller is faster
  timerAlarmEnable(step_timer);

  Serial.println("Server started");
}

void loop() {
  server.handleClient();
  delay(2);            //allow the cpu to switch to other tasks
  
  // moved this to the interrupt handler
  // step_the_motors();  // TODO: do this update with a timer in an interrupt handler
}
