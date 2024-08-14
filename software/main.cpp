//
// Alarm door bell (utilizing Swiss Post Horns)
//
// tj, March/April 2024
//

#include <Arduino.h>
//#include <avr/sleep.h>
//#include <avr/power.h>
#include <avr/wdt.h> 
#include "Vector.h"

//#define TEST       // for testing purposes only

#define alarmPort    A0
#define valveNone    NULL
#define valveOne     A8
#define valveTwo     A9
#define valveThree   A10

#define valveOpen   LOW
#define valveClosed HIGH

#define RXLED 17                 // the RX LED has a defined Arduino pin (D17, PB0)
//#define TXLED PORTD & (1<<5)   // not needed
// Note: we can use pre-defined macros (TXLED1, TXLED0, RXLED1, RXLED0)

#define USE_WDT                  // watchdog enabled
//#define USE_WDT_INTERRUPT      // Watchdog IRQ routine not used
#ifdef USE_WDT_INTERRUPT
ISR (WDT_vect)
{
  Serial.println(F("ISR(WDT_vect) routine executed"));
}
#endif

// melody settings

struct tone_t 
{
  const int valve;      // 0...3: valve 1...3, or 0 for none (valves closed)
  const int delay;      // ms: for how long is the valve activated (open)
};

// - the melody has 8 tones (+ 2 pause) alltogether, it's settings can be changed here
// - there are no further changes needed in the code down below !!
// - below are the final melody settings after thorough real world testing 

// ------------------------------>>>>>
tone_t tone_array[] = {
  { .valve = valveOne,   .delay = 400 },      // first tone
  { .valve = valveTwo,   .delay = 1050 },     // second tone
  { .valve = valveThree, .delay = 1050 },     // third tone
  { .valve = valveNone,  .delay = 130 },      // pause, all valves closed
  { .valve = valveOne,   .delay = 990 },      // forth tone
  { .valve = valveTwo,   .delay = 1020 },     // fifth ...
  { .valve = valveThree, .delay = 1040 },     // ...
  { .valve = valveNone,  .delay = 90 },       // pause, all valves closed
  { .valve = valveOne,   .delay = 970 },      //
  { .valve = valveTwo,   .delay = 920 },      //
}; 
// <<<<<------------------------------

// set variables
Vector<tone_t> melody(tone_array, sizeof(tone_array) / 4);  // actually sizeof() should be 32 for 8 tones 
int loopCounter = 0;
bool activated = true;

// needed to enable printf()
int serial_putc(char c, FILE *)
{
  Serial.write(c);
  return c;
}

// helper function
void ledBlink(uint8_t led, unsigned long ms) {
  for (unsigned long i = 0; i < ms;) {
    led == RXLED ? RXLED0 : TXLED0;
    delay(100);
    led == RXLED ? RXLED1 : TXLED1;
    delay(100);
    i += 200;
#ifdef USE_WDT
    wdt_reset();
#endif
  }
  RXLED0;
}

// initialize timer1 to generate interrupt every 10ms
// argument: time ... in sec (e.g. '0.01' for 10ms)
void initTimer1(float time)
{
  long cnt = 16000000 / 1024 * time;          // cnt = clk / prescaler * time(s)
  if (cnt > 65535) {
    cnt = 65535;                              // timer1 is a 16bit counter ! -> max. time is ~4.1s
  }

  cli();
  TCCR1A = 0;                                 // clear all default presets
  TCCR1B = 0;
  TCNT1 = 0;                                  // clear counter
  OCR1A = cnt;                                // set divider
  TCCR1B = bit(WGM12) | bit(CS12)| bit(CS10); // WGM12 => CTC (Clear Timer on Compare Match) and 
                                              // CS12 & CS10  => prescaler 1/1024 (64us)
  TIMSK1 |= (1 << OCIE1A);                    // OCIE1A => Timer1 compare match A interrupt
  sei();
}

// timer1 interrupt routine gets called every 10ms as set in setup()
ISR (TIMER1_COMPA_vect)
{
  ++loopCounter;
  // perform sign of life on RXLED
  if (loopCounter == 3 && !digitalRead(RXLED)) {
    // LED is on since 30ms
    RXLED0;
    loopCounter = 0;
  }
  else if (loopCounter == 197 && digitalRead(RXLED)) {
    // LED is off since 1970ms
    RXLED1;
    loopCounter = 0;
  }
}

// runs only once
void setup() {
  // LED initialization
  //pinMode(RXLED, OUTPUT);          // Set RX LED as output, nope...is done below
  TX_RX_LED_INIT;                    // set both LEDs in one go

  // printf() for the lazy folks
  fdevopen(&serial_putc, 0);
  Serial.begin(9600); 
  ledBlink(RXLED, 10000UL);          // within 10s the terminal prog should be connected to see startup output
  printf("\nSerial monitor has been initialized with: 9600,8,N,1.\n");

  // make sure all valves are closed
  digitalWrite(valveOne, valveClosed);
  pinMode(valveOne, OUTPUT);         // D8 --> Rel 1 --> Valve 1
  digitalWrite(valveTwo, valveClosed);
  pinMode(valveTwo, OUTPUT);         // D9 --> Rel 2 --> Valve 2
  digitalWrite(valveThree, valveClosed);
  pinMode(valveThree, OUTPUT);       // D10 --> Rel 3 --> Valve 3

  // input setting
  pinMode(alarmPort, INPUT_PULLUP);  // internal pullup in parallel to external one

  // print above set melody for information purposes only
  printf("Melody is defined as follows:\n number of tones: %d\n", melody.size());
  for (unsigned int i = 0; i < melody.size(); i++) {
    if (melody.at(i).valve)
      printf(" valve: %d open for %4dms\n", melody.at(i).valve - 25, melody.at(i).delay);
    else
      printf(" pause for %4dms\n", melody.at(i).delay);
  }

#ifdef USE_WDT
  // we use hardware watchdog
  wdt_reset();
  wdt_enable(WDTO_8S);
#ifdef USE_WDT_INTERRUPT
  WDTCSR = (1<<WDIE);             // enable WDT interrupt
#endif
#endif

  initTimer1(0.01);               // timer interrupt every 10ms
  printf("Waiting for alarm button getting pressed...\n");
#ifdef TEST
  printf("sizeof(tone_array) in bytes: %d\n", sizeof(tone_array));
  printf("melody.size(): %d\n", melody.size());
  printf("melody.max_size(): %d\n", melody.max_size());
#endif
}

// runs repeatedly
void loop() {
#ifndef TEST
  // production code
#ifdef USE_WDT
  wdt_reset();
#endif
  delay(10);
  // check alarm input port
  if (digitalRead(alarmPort) && !activated) activated = true;   // system gets rearmed
  if (!digitalRead(alarmPort) && activated) {
    // wait some ms to make sure it's not a random spike
    delay(50);
    if (digitalRead(alarmPort)) return;
    // button still active, so we start the action
    activated = false;
    printf("Button press detected. Alarm bell will sound.\n");
    for (int i = 0; i < (int)melody.size(); i++) {
      //printf(" tone %d: valve %d open for %4dms\n", i + 1, melody.at(i).valve - 25, melody.at(i).delay);
      if (melody.at(i).valve) {
        // activate valve
        digitalWrite(melody.at(i).valve, valveOpen);
        delay(melody.at(i).delay);
        digitalWrite(melody.at(i).valve, valveClosed);
      }
      else {
        // pause, all valves stay closed
        delay(melody.at(i).delay);
      }
#ifdef USE_WDT
      wdt_reset();
#endif
    } // for
  }
#else
  // code below just for testing purposes !!!!!!!!!!!!!!!!!!!!!!!

  printf("New Loop %d\n", ++loopCounter);    // print loop counter for Serial Monitor
  digitalWrite(A8, LOW);
  //digitalWrite(RXLED, LOW);      // set the RX LED ON
  RXLED1;                          // use predefined macro, turn RX LED ON
  TXLED0;                          // use predefined macro, turn TX LED OFF
  delay(1000);
  digitalWrite(A8, HIGH);
  //digitalWrite(RXLED, HIGH);     // set the RX LED OFF
  RXLED0;                          // use predefined macro, turn RX LED OFF
  TXLED1;                          // use predefined macro, turn TX LED ON
  delay(1000);                     // wait for a second
  digitalWrite(A9, LOW);
  delay(1000);
  digitalWrite(A9, HIGH);
  digitalWrite(A10, LOW);
  delay(1000);
  digitalWrite(A10, HIGH);
  dlay(1000);
#endif
}