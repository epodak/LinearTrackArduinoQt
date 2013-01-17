// TODO: - trigger sensors with interrupts to avoid missing crossings
//       due to ongoing serial communication
//      - move pins to arrays for neater code/shorter loop
//      - reduce number of bytes sent to PC (quite insane right now)


static int outLPin = 12 ; // choose the pin for the left solenoid
static int outRPin = 11; // choose the pin for the right solenoid
static int inLPin = 7;   // choose the input pin for left semsor
static int inRPin = 6;   // choose the input pin for right semsor
static float VERSION = 0.1; // firmware version currently on Arduino

static boolean ON = HIGH;
static boolean OFF = !ON;

int durL = 400; // duration left solenoid opens
int durR = 400; // duration right solenoid opens

boolean left = 0;     // variable for reading the pin status
boolean right = 0;

boolean nextLeft = 1;
boolean nextRight = 0;

byte next = outLPin;

byte incomingByte;

void setup() {
  pinMode(outLPin, OUTPUT);  // declare left solenoid pin as output
  pinMode(outRPin, OUTPUT);  // declare right solenoid pin as output
  pinMode(inLPin, INPUT);    // declare left sensor as input
  pinMode(inRPin, INPUT);    // declare right sensor as input
  digitalWrite(outLPin, LOW); // turn solenoids OFF
  digitalWrite(outRPin, LOW);  // turn solenoids OFF
  
  Serial.begin(57600);
}

void loop(){
    left = digitalRead(inLPin);  // read input value
    right = digitalRead(inRPin);  // read input value

  // First send information to PC, trigger will happen anyway,
  // but might be delayed a few seconds, which is no issue. The
  // other way round the sensor may have closed already without the 
  // change being displayed, as the loop waits for the solenoid

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 86) {
      Serial.println(VERSION);
    }
  }
  
  // send current state as single byte to PC
  Serial.write(((!left)<<0)+((!right)<<1)); // LSB = Right sensor,2nd LSB = Left Sensor; Extend as needed

  if (next == outRPin){
    if (right == ON && left == OFF) {         // check if the input is HIGH (button released)
      digitalWrite(next, HIGH);  // turn LED ON
      delay(durR);
      digitalWrite(next, LOW);
      next = outLPin;
    } else {
      digitalWrite(next, LOW);  // turn LED OFF
    }
  } else {
    if (left == ON && right == OFF) {         // check if the input is HIGH (button released)
      digitalWrite(next, HIGH);  // turn LED ON
      delay(durL);
      digitalWrite(next, LOW);
      next = outRPin;
    } else {
      digitalWrite(next, LOW);  // turn LED OFF
    }
  }
}

