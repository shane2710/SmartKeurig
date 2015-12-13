/////////////////////////////////////////
// Author: Shane Ryan
// Purpose: Control Keurig K40
//
// Nine wires run out the back of
// the Keurig. All are soldered to
// respective joints on the input
// board to control the Keurig, emulating
// button presses w/ quick 5V pulses.
//////////////////////////////////////////


/* note:  to add to functionality, I could make a database that
 *  stores the number of keurig credits someone in the dorm has,
 *  then they can electronically request coffee, a reloading mechanism
 *  can be implemented, people can buy credits from the dorm, a queue 
 *  and graphical front end can be developed, and a pi-zero could control
 *  each floor's keurig (using a shift register to connect its few GPIO pins if 
 *  need be) and a small LCD installed on top!! That'd be a cool system.
 *  Smart Keurig network, dorm wide.  Heck, why not campus wide?
 *  
 *  Coudl make.... Keurig vending machines!!  Smart ones.  Walking around campus,
 *  front-end shows user closest machine and its queue, then can order online
 *  with domer dollars.  Or flex points. 
 */

// #include <Timer.h>
// #include <Event.h>

// states declared:
const int ADD_WATER = 1;
const int DESCALE = 2;
const int SUCCESS = 0;

// note: #define is not used in order to
// avoid accidental replacement of 'add_water'
// within 'add_water_pin' in some older versions.

/* pins for digital output, simulating
 *  buttons presses:
 */
const int power_pin = 5;
const int open_close = 6;
const int small_brew = 4;
const int med_brew = 3;
const int large_brew = 2;

/* analog pins declared for reading inputs
 *  as voltages:
 */
const int add_water_pin = 5;
const int descale_pin = 3;
const int heating_pin = 4;

/* Variable to track state of Keurig, on or standby,
 * by flipping every time the button 5 is pressed, and
 * making the assumption that the Arduino is powered on with the 
 * Keurig, dependently.  Global variable for this reason.
 */
int power_state = -1;
int waiting = 0;



void setup() {  
  // setup pins for digital output
  for (int i = 2; i < 7; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  // note: no setup needed for the three analog input pins

  Serial.begin(9600);  // configure baud rate - 9600 bits per second
  digitalWrite(6, HIGH); // the open_close switch is NC (normally closed)
  
}



void loop() {
  int end_result = 0;
  int brew_size = 0;
  
  // check if we've already printed "waiting..."
  if (waiting == 0) { 
    Serial.println("Waiting for request...");
    waiting = 1;
  }

  while (Serial.available()) {
    waiting = 0; // reset waiting variable
    brew_size = (Serial.read() - '0');
    if (!brew_size) {
      press_button(power_pin);
    }
    else {
      Serial.println("Received brew request!");
      Serial.println();
      Serial.println("Size: ");

      switch (brew_size) {
        case 2:
            Serial.println("Large");
            break;
        case 3:
            Serial.println("Medium");
            break;
        case 4:
            Serial.println("Small");
            break;
      }
      Serial.println("Starting...");
      Serial.println();
    
      end_result = makeCoffee(brew_size);
    
      switch (end_result) {
        case SUCCESS:
          Serial.println("Everything went A-OK!");
          break;
        case ADD_WATER:
          Serial.println("Need more water!");
          break;
        case DESCALE:
          Serial.println("Time for a cleaning!");
          break;
        default:
          Serial.println("Unknown error, self-destructing...");
          break;
      }
    }
  }
  delay(500);
}



int makeCoffee(int selection)
{
  int ready_status = 0;
  int final_brew_check = 0;
  
  press_button(power_pin);
  delay(3000);

  // check to see if we're ready to brew:
  ready_status = brew_status();
  if (ready_status) {
    return ready_status;
  }
  delay(1500);

  // also, wait while the keurig is heating up:
  while ((analogRead(heating_pin) * (5.0 / 1023)) < 4) {
    Serial.println("Heating...");
    // add code to print to small lcd as well.
    delay(1000);
  }

  /* here is a snippet to run the inverted logic
   *  needed to simulate the open and close:
   */
  Serial.println("Pressed button: 6");
  digitalWrite(6, LOW);
  delay(500);
  digitalWrite(6, HIGH);
  delay(2000);

  // sometimes the keurig heats now as well
  while ((analogRead(heating_pin) * (5.0 / 1023)) < 4) {
    Serial.println("Heating...");
    // add code to print to small lcd as well.
    delay(1000);
  }

  /* Now we can choose the brew size! */
  press_button(selection);
    
  /* need code here to read size LEDs and determine if
   *  brewing has finished.  For now, this will just be
   *  a 45 second timer.  (INTERRUPTS!?)
   */ 
   
  Serial.println("Brewing...");
  delay(40000);  // wait for now to determine if brew is done.

  final_brew_check = brew_status(); // check statusd
  press_button(power_pin); // turn machine back off

  return final_brew_check; // return status
}



void press_button(int j)
{
  digitalWrite(j, HIGH);
  delay(100);
  digitalWrite(j, LOW);

  /* check to see if the Keurig is ON or in STANDBY, relay
     the information, and flip power_state.
     */

  if (j == power_pin) {
      if (power_state == 1) {
        Serial.println("Keurig was ON, sending into STANDBY...");
        Serial.println();
        power_state = 0;
      }
      else {
        Serial.println("Keurig was in STANDBY, turning ON...");
        Serial.println();
        power_state = 1;

      }
  }

  Serial.print("Pressed button: ");
  Serial.println(j, DEC);

}



int brew_status()
{
    /* using analog voltages on input pin of status LEDs,
   *  interpret the keurig's state and return an exit
   *  status:
   */

  if ((analogRead(descale_pin) * (5.0 / 1023)) < 4) {
    return DESCALE;
  }
  
  else if ((analogRead(add_water_pin) * (5.0 / 1023)) < 4) {
    return ADD_WATER;
  }

  else {
    return SUCCESS;
  }

  /* descaling is rare, and includes changing the water out,
   *  so we can just check for that first, as adding water
   *  will be necessary and therefore checking add_water is
   *  redundant.
   */
}

