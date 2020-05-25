// HVSP-FUSE-resetter for attiny85 compatibles
// tested with attiny85 in both roles as rescuer and victim
// inspired by https://sites.google.com/site/wayneholder/attiny-fuse-reset
// as a reduced form of
// and http://www.rickety.us/wp-content/uploads/2010/03/hv_serial_prog.pde
// adapted to all informations from the data-sheet of attiny85 as
// both implementations differed.

// Version 3.0
// 14. August 2013 by gogol


// defining which PIN will be used for which role

#define  DATAIN   1   // Target Data Input
                  //->connect to PIN7 (SDO) via 1k resistor
                   // on-board-LED would indicate the result of fuseReset
#define  RST     5   // connect to 12V switch-Resistor for HVS-programming, 
                   // limited current of PB5 no problem for that purpose
                  //->connect via 10k resistor to basis of transistor to PIN1 (RST)
                   // Note: The Reset function on PB5 must be disabled, thus PB5 can be a GPIO to trigger reset on target chip.
#define  VCC     2   // Target VCC 
                  //->connect to PIN8 (Vcc) direct
#define  INSTOUT  4   // Target Instruction Input
                   // limited high of 3.6V because of zener no problem, as detected as high
                  //->connect to PIN6 (SII) via 1k resistor
                   // If PB4 is high during start-up, the whole chip would be erased to reset lockbits.Please release the key-swith after the twice-flash of the on-board-LED.
#define  CLKOUT   3   // Target Clock Input
                   // limited high of 3.6V because of zener and pullup no problem, as detected as high
                  //->connect to PIN2 (SCI) via 1k resistor
#define  DATAOUT  0   // Target Data Output
                  //->connect to PIN5 (SDI) via 1k resistor

// fuses for attiny85 digispark-compatibel use, without RSTDISBL
// avrdude parameters: -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m
#define  HFVAL  0xdf
#define  LFVAL  0xf1
#define  EFVAL  0xfe

// byte constants for HVSP-programming instructions
// see attiny85 datasheet table 20-16
#define _0_0000_0000_00 0b00000000
#define _0_0000_0100_00 0b00000100
#define _0_0000_1000_00 0b00001000
#define _0_0000_1100_00 0b00001100
#define _0_0010_1100_00 0b00101100
#define _0_0100_0000_00 0b01000000
#define _0_0100_1100_00 0b01001100
#define _0_0110_0100_00 0b01100100
#define _0_0110_0110_00 0b01100110
#define _0_0110_1000_00 0b01101000
#define _0_0110_1010_00 0b01101010
#define _0_0110_1100_00 0b01101100
#define _0_0110_1110_00 0b01101110
#define _0_0111_0100_00 0b01110100
#define _0_0111_1000_00 0b01111000
#define _0_0111_1010_00 0b01111010
#define _0_0111_1100_00 0b01111100
#define _0_0111_1110_00 0b01111110
#define _0_1000_0000_00 0b10000000


boolean fuse_ok      = false;
boolean lock_bit_set = false;
boolean eraseChip    = false;

void setup() {
  // all work is done in setup()
  // fuse resetting either works or fails
  // The following instructions are from the datasheet:
  // 1. Set Prog_enable pins listed in Table 20-14 to “000”, RESET pin and VCC to 0V.
  // 2. Apply 4.5 - 5.5V between VCC and GND. Ensure that VCC reaches at least 1.8V within the next 20 μs.
  // 3. Wait 20 - 60 μs, and apply 11.5 - 12.5V to RESET.
  // 4. Keep the Prog_enable pins unchanged for at least 10 μs after the High-voltage has been applied to
  //   ensure the Prog_enable Signature has been latched.
  // 5. Release the Prog_enable[2] pin to avoid drive contention on the Prog_enable[2]/SDO pin.
  // 6. Wait at least 300 μs before giving any serial instructions on SDI/SII.
  // 7. Exit Programming mode by power the device down or by bringing RESET pin to 0V.

  
  pinMode(VCC, OUTPUT); 
  digitalWrite(VCC, LOW);      // make sure, no +5V on target
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);     // transistor is inverting signal, this shuts off 12V
  pinMode(CLKOUT, OUTPUT);
  digitalWrite(CLKOUT, LOW);   
  pinMode(DATAOUT, OUTPUT);    
  digitalWrite(DATAOUT, LOW);
  pinMode(DATAIN, OUTPUT);     // temporary for fullfilling 1.)
  digitalWrite(DATAIN, HIGH); 
 delay(50);
  digitalWrite(DATAIN, LOW);
 delay(100);
  digitalWrite(DATAIN, HIGH);
 delay(50);
  digitalWrite(DATAIN, LOW);  // Flash twice to indicate Bootloader over and fuseReset process start

  pinMode(INSTOUT, INPUT);
  if (digitalRead(INSTOUT)) {
    eraseChip = true;        // when INSTOUT is high at startup, the whole chip should be erased to reset lockbits. 
  };
  while (digitalRead(INSTOUT)); // make sure, the key is not longer pressed.
 delay(500);

  pinMode(INSTOUT, OUTPUT);     // now set INSTOUT as well to OUTPUT!
  digitalWrite(INSTOUT, LOW);

  digitalWrite(VCC, HIGH);     // 2.) switch target Vcc on
 delayMicroseconds(50);
  digitalWrite(RST, LOW);      // 3.) 12v powered ON
  delayMicroseconds(15);       // 4.)
  pinMode(DATAIN, INPUT);      // 5.) Set DATAIN to input
  delayMicroseconds(330);      // 6.)
  if (eraseChip) {
   chipErase();
  }  
  if (( 0b00000110 & readLockBits()) != 0b00000110) {
   lock_bit_set = true; 
  }  else {
   writeFuse(LFVAL, _0_0110_0100_00, _0_0110_1100_00);
   writeFuse(HFVAL, _0_0111_0100_00, _0_0111_1100_00);
   writeFuse(EFVAL, _0_0110_0110_00, _0_0110_1110_00);
    fuse_ok = checkFuses();
  }  

  digitalWrite(CLKOUT, LOW);
  digitalWrite(VCC, LOW);      // 7.) 
  digitalWrite(RST, HIGH);     //    
  pinMode(DATAIN, OUTPUT);     // change to output, so loop can blink codes
  digitalWrite(DATAIN, LOW); 

 
}

void clockPulse() {
  digitalWrite(CLKOUT, HIGH);
  digitalWrite(CLKOUT, LOW); 
}  

int hvsp_bytes_out( byte val, byte val1)
{
  int inBits = 0;
  // Wait until DATAIN goes high
  while (!digitalRead(DATAIN));  // now we are ready
  // each instruction command has 11 clock pulses
  // one before each of the 8bits, one after each bit, two after each byte
  //initial clock pulse while DATAOUT and INSTOUT LOW
  digitalWrite(DATAOUT, LOW);
  digitalWrite(INSTOUT, LOW);
 clockPulse();
  int i;
  // now the 8 databits
  for (i = 0; i < 8; i++)  {
   digitalWrite(DATAOUT, !!(val & (1 << (7 - i))));
   digitalWrite(INSTOUT, !!(val1 & (1 << (7 - i))));
    // a given return byte starts one bit before output bits!
    // therefore reading it before clock-pulse for out
    inBits <<=1;
    inBits |= digitalRead(DATAIN);
   clockPulse();  
  }
  // after each byte-group (one byte to DATAOUT and INSTOUT in parallel)
  // two clock-pulses with DATAOUT and INSTOUT LOW
  digitalWrite(DATAOUT, LOW);
  digitalWrite(INSTOUT, LOW);
 clockPulse();
 clockPulse();
  // return the 8-bits read 
  // interpretation see datasheet
  return inBits;
}

void writeFuse( byte value, byte instrc3, byte instrc4) {
  hvsp_bytes_out( _0_0100_0000_00, _0_0100_1100_00);
  hvsp_bytes_out( value,         _0_0010_1100_00);
  hvsp_bytes_out( _0_0000_0000_00, instrc3);
  hvsp_bytes_out( _0_0000_0000_00, instrc4);
  // Wait after Instr. 4 until SDO goes high
  while (!digitalRead(DATAIN));
}  

byte readFuse(byte instrc2, byte instrc3) {
  hvsp_bytes_out( _0_0000_0100_00, _0_0100_1100_00);
  hvsp_bytes_out( _0_0000_0000_00, instrc2);
  return hvsp_bytes_out( _0_0000_0000_00, instrc3);
}  

byte readLockBits( ) {
  hvsp_bytes_out( _0_0000_0100_00, _0_0100_1100_00);
  hvsp_bytes_out( _0_0000_0000_00, _0_0111_1000_00);
  return hvsp_bytes_out( _0_0000_0000_00, _0_0111_1100_00);
}  

void chipErase( ) {
  hvsp_bytes_out( _0_1000_0000_00, _0_0100_1100_00);
  hvsp_bytes_out( _0_0000_0000_00, _0_0110_0100_00);
  hvsp_bytes_out( _0_0000_0000_00, _0_0110_1100_00);
  // Wait after Instr. 3 until SDO goes high
  while (!digitalRead(DATAIN));
}  

boolean checkFuses () {
  if (LFVAL != readFuse(_0_0110_1000_00, _0_0110_1100_00)) {
    return false; 
  }  
  if (HFVAL != readFuse(_0_0111_1010_00, _0_0111_1110_00)) {
    return false; 
  }  
  // the only guaranteed value is the first bit
  // all other values the datasheet tells: "don't care!"
  if ((0b00000001 & EFVAL) != ( 0b00000001 &  readFuse(_0_0110_1010_00, _0_0110_1110_00))) {
    return false; 
  }  
  return true;
}


void loop() {
  if (fuse_ok) { // LED keeps on, when fusing was ok
     digitalWrite(DATAIN, HIGH); 
  } else if (lock_bit_set) {
     digitalWrite(DATAIN, HIGH); // LED blinking slowly, when lock-bit set. If you want to reset the lockbits and erase the chip, You need to press the key-swith at power-up, then release after the twice-flash of the on-board-LED.
     delay(1000);
     digitalWrite(DATAIN, LOW); 
     delay(1000);
  } else {
     digitalWrite(DATAIN, HIGH); // LED blinking rapidly, when error in fusing
     delay(50);
     digitalWrite(DATAIN, LOW); 
     delay(50);
  }  
}  // Note : on-board-LED would not burn or flash when target chip don't respond.  e.g. condition like bad chip or low voltage of 12V.
