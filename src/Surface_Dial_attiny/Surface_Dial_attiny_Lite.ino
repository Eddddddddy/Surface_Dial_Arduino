#include "TrinketHidCombo.h"

#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 1
#define TRINKET_PINx  PINB
#define PIN_ENCODER_SWITCH 0

static uint8_t enc_prev_pos   = 0;
static uint8_t enc_flags      = 0;
static char    sw_was_pressed = 0;


int speed_r = 3;
uint8_t rotate_r_R = DIAL_R;
uint8_t rotate_r_L = DIAL_L;

volatile bool previousButtonValue = false, lastButtonValue = false;

volatile int previous = 0;
volatile int counter = 0;

long debounceDelay = 50;
long lastDebounceTime = 0;

long lastDouble = 0;
long nowDouble = 0;
long ddlDouble=500;



void setup()
{
  // set pins as input with internal pull-up resistors enabled
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);
//  digitalWrite(PIN_ENCODER_A, HIGH);
//  digitalWrite(PIN_ENCODER_B, HIGH);

  pinMode(PIN_ENCODER_SWITCH, INPUT);
  // the switch is active-high, not active-low
  // since it shares the pin with Trinket's built-in LED
  // the LED acts as a pull-down resistor
//  digitalWrite(PIN_ENCODER_SWITCH, LOW);

  TrinketHidCombo.begin(); // start the USB device engine and enumerate

  // get an initial reading on the encoder pins
  if (digitalRead(PIN_ENCODER_A) == LOW) {
    enc_prev_pos |= (1 << 0);
  }
  if (digitalRead(PIN_ENCODER_B) == LOW) {
    enc_prev_pos |= (1 << 1);
  }
}


void doubleClick() {
  if (rotate_r_R == DIAL_R) {
    rotate_r_R = DIAL_R_F;
    rotate_r_L = DIAL_L_F;
    speed_r = 1;
  } else if (rotate_r_R == DIAL_R_F) {
    rotate_r_R = DIAL_R;
    rotate_r_L = DIAL_L;
    speed_r = 3;
  }
}

void loop()
{
  int8_t enc_action = 0; // 1 or -1 if moved, sign is direction

  // note: for better performance, the code will now use
  // direct port access techniques
  // http://www.arduino.cc/en/Reference/PortManipulation
  uint8_t enc_cur_pos = 0;
  // read in the encoder state first
  if (bit_is_clear(TRINKET_PINx, PIN_ENCODER_A)) {
    enc_cur_pos |= (1 << 0);
  }
  if (bit_is_clear(TRINKET_PINx, PIN_ENCODER_B)) {
    enc_cur_pos |= (1 << 1);
  }

  // if any rotation at all
  if (enc_cur_pos != enc_prev_pos)
  {
    if (enc_prev_pos == 0x00)
    {
      // this is the first edge
      if (enc_cur_pos == 0x01) {
        enc_flags |= (1 << 0);
      }
      else if (enc_cur_pos == 0x02) {
        enc_flags |= (1 << 1);
      }
    }

    if (enc_cur_pos == 0x03)
    {
      // this is when the encoder is in the middle of a "step"
      enc_flags |= (1 << 4);
    }
    else if (enc_cur_pos == 0x00)
    {
      // this is the final edge
      if (enc_prev_pos == 0x02) {
        enc_flags |= (1 << 2);
      }
      else if (enc_prev_pos == 0x01) {
        enc_flags |= (1 << 3);
      }

      // check the first and last edge
      // or maybe one edge is missing, if missing then require the middle state
      // this will reject bounces and false movements
      if (bit_is_set(enc_flags, 0) && (bit_is_set(enc_flags, 2) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }
      else if (bit_is_set(enc_flags, 2) && (bit_is_set(enc_flags, 0) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }
      else if (bit_is_set(enc_flags, 1) && (bit_is_set(enc_flags, 3) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }
      else if (bit_is_set(enc_flags, 3) && (bit_is_set(enc_flags, 1) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }

      enc_flags = 0; // reset for next time
    }
  }

  enc_prev_pos = enc_cur_pos;

  if (enc_action > 0) {
        TrinketHidCombo.pressMultimediaKey(rotate_r_R);
        //Serial.println("R");
        //counter -= speed_r;
  }
  else if (enc_action < 0) {
    TrinketHidCombo.pressMultimediaKey(rotate_r_L);
        //Serial.println("L");
        //counter += speed_r;
  }

//  // remember that the switch is active-high
  if (bit_is_set(TRINKET_PINx, PIN_ENCODER_SWITCH)) 
  {
    if (sw_was_pressed == 0) // only on initial press, so the keystroke is not repeated while the button is held down
    {
      TrinketHidCombo.pressMultimediaKey(DIAL_RELEASE);
      delay(50); // debounce delay
    }
    sw_was_pressed = 1;
  }
  else
  {
    if (sw_was_pressed != 0) {
      TrinketHidCombo.pressMultimediaKey(DIAL_PRESS);
      delay(50); // debounce delay
              lastDouble = nowDouble;
        nowDouble = millis();
        if((nowDouble-lastDouble)<ddlDouble){
          doubleClick();
          nowDouble=0;
        }
    }
    sw_was_pressed = 0;
  }

//bool buttonValue = digitalRead(PIN_ENCODER_SWITCH);
//  bool buttonValue = bit_is_set(TRINKET_PINx, PIN_ENCODER_SWITCH);
//  if (buttonValue != lastButtonValue) {
//    lastDebounceTime = millis();
//  }
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    if (buttonValue != previousButtonValue) {
//      previousButtonValue = buttonValue;
//      if (buttonValue) {
//        TrinketHidCombo.pressMultimediaKey(DIAL_PRESS);
//          digitalWrite(TAPTIC,HIGH);
//          delay(Taptic_r);
//          digitalWrite(TAPTIC,LOW);
//        lastDouble = nowDouble;
//        nowDouble = millis();
//        if((nowDouble-lastDouble)<ddlDouble){
//          doubleClick();
//          nowDouble=0;
//        }
//      } else {
//        TrinketHidCombo.pressMultimediaKey(DIAL_RELEASE);
//
//      }
//    }
//  }

  

  TrinketHidCombo.poll(); // check if USB needs anything done
}
