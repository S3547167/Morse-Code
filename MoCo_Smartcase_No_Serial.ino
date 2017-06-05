// Output pin from button to D2
// Ground to ground
// Power to 3V
//
//#include "Adafruit_SleepyDog.h"
#include "keyboardSetup.h"
String KEYBOARD_NAME = "MoCo Smartcase";

const int dotPin = A0; // Dot button to pin 2
const int dashPin = A1; // Dash button to pin 3
const int spacePin = A2; // Space/Backspace button to pin 5

int pressLength = 0;
//
//const int shortSig = 100; // Dot = 1
//const int longSig = 500; // Dash = 2
//const int pauseSig = 500; // No input = 0

bool arrayNum = 0;

int stream[5] = { -1, -1, -1, -1, -1}; // Begin with no input in the stream


const char alphabet[2][46] = {
  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '!', '*', '"', '`', ' ', '\b', 0x28},
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '!', '*', '"', '`', ' ', '\b', 0x28}
};

const int combos[46][5] = {{1, 2, 0, 0, 0}, {2, 1, 1, 1, 0}, {2, 1, 2, 1, 0}, {2, 1, 1, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 2, 1, 0}, {2, 2, 1, 0, 0}, {1, 1, 1, 1, 0}, {1, 1, 0, 0, 0}, {1, 2, 2, 2, 0}, {2, 1, 2, 0, 0}, {1, 2, 1, 1, 0}, {2, 2, 0, 0, 0}, {2, 1, 0, 0, 0}, {2, 2, 2, 0, 0}, {1, 2, 2, 1, 0}, {2, 2, 1, 2, 0}, {1, 2, 1, 0, 0}, {1, 1, 1, 0, 0}, {2, 0, 0, 0, 0}, {1, 1, 2, 0, 0}, {1, 1, 1, 2, 0}, {1, 2, 2, 0, 0}, {2, 1, 1, 2, 0}, {2, 1, 2, 2, 0}, {2, 2, 1, 1, 0}, {2, 2, 2, 2, 2}, {1, 2, 2, 2, 2}, {1, 1, 2, 2, 2}, {1, 1, 1, 2, 2}, {1, 1, 1, 1, 2}, {1, 1, 1, 1, 1}, {2, 1, 1, 1, 1}, {2, 2, 1, 1, 1}, {2, 2, 2, 1, 1}, {2, 2, 2, 2, 1}, {3, 1, 0, 0, 0}, {3, 2, 0, 0, 0}, {3, 1, 1, 0, 0}, {3, 2, 2, 0, 0}, {3, 1, 2, 0, 0}, {3, 2, 1, 1, 0}, {3, 2, 1, 0, 0}, {3, 0, 0, 0, 0}, {3 , 3 , 0 , 0 , 0}, {3, 3, 3, 0, 0}};

int start = 0;

int inSig;

void setup() {
  //  while (!Serial);  // wait for Serial to startup
  //  delay(500);
  //  Serial.begin(115200);
  //  Serial.println("Basic Chorded Keyboard");
  //  Serial.println("---------------------------------------");

  setupKeyboard(KEYBOARD_NAME);

  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(spacePin, INPUT_PULLUP); // Was getting random inputs from the button but we fixed it with a pullup
}

void loop() {

  while (start == 0) {
    if (digitalRead(dotPin) == LOW || digitalRead(dashPin) == LOW || digitalRead(spacePin) == LOW) { //Makes it only run when it needs to
      start = 1;
    }
  }

  if (digitalRead(dotPin) == LOW) {  
    inSig = 0;
    while (digitalRead(dotPin) == LOW) {
      delay(50);
      inSig += 50; // Whenever it is pressed it will add 50ms to the insig every time the indig loops 50ms
    }

    if (inSig >= 200) {
      //      Serial.println("- Reset");
      inSig = 0;
      streamreset();
    } else if (inSig >= 50) { // If button is pushed for 50ms but less than 1000ms push a 1 to the stream
      streampush(1);
    }
  }

  if (digitalRead(dashPin) == LOW) {
    inSig = 0;
    while (digitalRead(dashPin) == LOW) {
      delay(50);
      inSig += 50;
    }

    if (inSig >= 200) {
      //      Serial.print(" ");
      inSig = 0;
      streamreset();
    } else if (inSig >= 50) { // If button is pushed for 50ms but less than 1000ms push a 2 to the stream
      streampush(2);
    }
  }


  if (digitalRead(spacePin) == LOW) {
    inSig = 0;
    while (digitalRead(spacePin) == LOW) {
      delay(50);
      inSig += 50;
    }

    if (inSig >= 200) {
      //      Serial.print(" ");
      inSig = 0;
      streamreset();

      arrayNum = 1;

    } else if (inSig >= 50) { // If button is pushed for 50ms but less than 1000ms push a 1 to the stream
      streampush(3);
    }
  }

  if (digitalRead(dotPin) == HIGH && digitalRead(dashPin) == HIGH && digitalRead(spacePin) == HIGH) {
    while (digitalRead(dotPin) == HIGH && digitalRead(dashPin) == HIGH && digitalRead(spacePin) == HIGH) {
      delay(100);
      inSig -= 100;
      if (inSig <= -500) {
        while (!streamcheck()) { // while streamcheck isn't full, push a 0 to the stream every 500ms
          streampush(0); 
        }
        break;
      }
    }
  }
  if (streamcheck()) {
    streaminterp();
    streamreset(); //if streamcheck is true, interpret and reset
  }
}

void streampush(int input) {
  //  Serial.print("Pushing Stream: "); // Puts the stream input into the correct orientation
  //  Serial.println(input);
  stream[4] = stream[3];
  stream[3] = stream[2];
  stream[2] = stream[1];
  stream[1] = stream[0];
  stream[0] = input; //shifts previous inputs to the right when a new number is pushed to the stream
}

bool streamcheck() { //checks to see if there are 5 in the stream
  for (int i = 0; i < 5; ++i) { //everytime this loops and is less than 5 through add 1 to i
    if (stream[i] == -1) {
      return false;
    }
  }
  return true; //
}

void streaminterp() { // This is was makes the combination reference the letter in the alaphabet array
  //  Serial.print("Interpreting Stream: ");
  for (int i = 0; i < 46; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (stream[4 - j] != combos[i][j]) { //checks to see if the stream lines up starting from the end
        break;
      }
      if (j == 4) {
        sendChar(alphabet[arrayNum][i]);
        //        Serial.println(alphabet[i]);
        arrayNum = 0; //take from lowercase array
        return;
      }
    }
  }
}

void streamreset() { // Reset the stream
  for (int i = 0; i < 5; ++i) {
    stream[i] = -1;
  }
  start = 0;
}

void sendChar(char theChar) {
  //  Serial.print("Sending: ");
  //  Serial.println(theChar);
  if (theChar == 0x28) {
    ble.print("AT+BLEKEYBOARDCODE=");
    ble.println("00-00-28-00-00"); // BLE code for Return/send

    //      if ( ble.waitForOK() )
    //      {
    ////        Serial.println( F("OK!") );
    //      } else
    //      {
    //        Serial.println( F("FAILED!") );
    //        // Failed, probably pairing is not complete yet
    //        Serial.println( F("Please make sure Bluefruit is paired and try again") );
    //      }
    //
     ble.print("AT+BLEKEYBOARDCODE=");
     ble.println("00-00");
    }
    else {
     ble.print("AT+BleKeyboard=");
     ble.println(theChar);
    //    //  wait for feedback
    //    if ( ble.waitForOK() )
    //    {
    //      Serial.println("OK!");
    //    } else
    //    {
    //      Serial.println("FAILED!");
    //    }
    //  }
  }
}
//  int sleepMS = Watchdog.sleep(5000);

