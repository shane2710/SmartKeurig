//////////////////////////////////////////
// Author: Shane Ryan
// Purpose: Control Keurig K40
//
// Nine wires run out the back of
// the Keurig. All are soldered to
// respective joints on the input
// board to control the Keurig, emulating
// button presses w/ quick 5V pulses.
//////////////////////////////////////////


int power = 5;
int open_close = 6;
int small_brew = 4;
int med_brew = 3;
int large_brew = 2;
  
// char add_water_pin[] = {A5};
// char descale_pin[] = {A4};
// char heating_pin[] = {A3};

int add_water = 0;
int descale = 0;
int heating = 0;

int button = 0;
int i;

byte buffer1[] = {0};

void setup() {
  // pinMode(10, INPUT);
  // pinMode(11, INPUT);
  // pinMode(9, INPUT);

  for (i = 2; i < 7; i++) {
    pinMode(i, OUTPUT);
  }

  
  for (i = 2; i < 7; i++) {
    digitalWrite(i, LOW);
  }

  Serial.begin(9600);
  digitalWrite(6, HIGH);
}


void loop() {

  while (Serial.available()) {
    // Serial.readBytes(buffer1, 2);
    // Serial.println(buffer1[0]);
    // Serial.println(buffer1[1]);
    button = (Serial.read() - '0');
    if (button == 6) {
      digitalWrite(6, LOW);
      delay(500);
      digitalWrite(6, HIGH);
      Serial.print("6\n");
    }
    else {
    press_button(button);
    }
  }

  add_water = (analogRead(A5) * (5.0 / 1023));
  heating = (analogRead(A4) * (5.0 / 1023));
  // Serial.println(add_water);
  // Serial.println(heating);
  // Serial.println();
  
  if (add_water < 4) {
      Serial.println("add water!");
  }
  else {
      Serial.println("water ready");
  }

  if (heating < 4) {
      Serial.println("heating....");
  }
  else {
      Serial.println("heat ready!");
  }

  Serial.println();

  if ((add_water == 5) && (heating == 5)) {
      Serial.println("All systems ready!");
  }
  else {
      Serial.println("Check status!");
  }

  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println();
  

}


void press_button(int j)
{
  digitalWrite(j, HIGH);
  delay(100);
  digitalWrite(j, LOW);
  Serial.print("Pressed button: ");
  Serial.println(j, DEC);
}

