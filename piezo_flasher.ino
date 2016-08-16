/*
Piezo Flasher
For Piezo Flasher Hardware v1.1

Modifications needed for v1.1 hardware:
- replace D1 with 0R
- add jumper from MODE0 to PIXEL

Copyright (c) 2016 Shunya Sato
Author: Shunya Sato

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <avr/sleep.h>
#include <Adafruit_NeoPixel.h>
#include "sheet_music.h"

// Utility macros to reduce current consumption
// http://www.technoblogy.com/show?KX0
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define DEBUG true
#define SOUND true  // true: turn sound on, false: turn sound off for testing silently

#define PIXEL_COUNT 6

const int piezoPin = 3;
const int pixelPin = 10; // somehow A0 doesn't work...??
// A0 works fine on Arduino Uno. But not on ATmega328 on a breadboard with 8MHz internal clock
// v1.1 hardware must implement jumper wire from Pin 10 to A0
const int pixPWRPin = A1;
const int modePins[] = {6, 5}; // use pin 10 for neopixel

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, pixelPin, NEO_GRB + NEO_KHZ800);
int brightness = 64;

#define DELTA_PLUS 1
#define DELTA_MINUS 1
int wheelVal = 0;

int* songs[] = {theEnd, marioMain, marioUnderworld, mario1Up, marioPowerUp, marioCoin, totoro, geragera, yukidaruma, yume_dora, shaka};
int* tempos[] = {theEnd_tempo, marioMain_tempo, marioUnderworld_tempo, mario1Up_tempo, marioPowerUp_tempo, marioCoin_tempo, totoro_tempo, geragera_tempo, yukidaruma_tempo, yume_dora_tempo, shaka_tempo};
int lens[] = {theEnd_length, marioMain_length, marioUnderworld_length, mario1Up_length, marioPowerUp_length, marioCoin_length, totoro_length, geragera_length, yukidaruma_length, yume_dora_length, shaka_length};
int nsongs = sizeof(songs)/sizeof(int);

enum songsType {
  THE_END = 0,
  MARIO_MAIN,
  MARIO_UNDERWORLD,
  MARIO_1UP,
  MARIO_POWER_UP,
  MARIO_COIN,
  TOTORO,
  GERAGERA,
  YUKIDARUMA,
  YUME,
  SHAKA
};
int chimes[] = {MARIO_1UP, MARIO_POWER_UP, MARIO_COIN};
int nchimes = sizeof(chimes)/sizeof(int);

enum states {
  PLAY_ONE_RANDOM = 0,// check orientation for the 2nd time
  COIN_ONEUP, // 1
  PLAY_BY_NOTE_SHAKA, // 2
  PLAY_BY_NOTE, // 3: default
  PLAY_ONE_SEQUENCE,
  ONEUP,
  COIN_ONEUP_RAMDOM,
  CHIMES_RANDOM,
  RACE_10SEC,
  PLAY_ALL,
  PLAY_ONE
};
enum states state = COIN_ONEUP;
enum states prev_state = COIN_ONEUP;
int nstates = 10;

unsigned long debounceTime = 0;
int counti = 0;
int songi = (int)MARIO_MAIN; // MARIO_MAIN
unsigned long gameStart = 0;
unsigned long lastPlayed = 0;
int kirakira = 0;

void sleep_isr(){
  // isr to wake up from sleep
  sleep_disable();
  detachInterrupt(1);
}

void go_sleep(){
  Serial.println(F("Going to sleep!"));
  lightOFF();
  digitalWrite(pixPWRPin, HIGH);  // cut power for neopixel
  detachInterrupt(1);
  noInterrupts(); // disable interrupts i.e. cli();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable(); // Set the SE (sleep enable) bit.
  attachInterrupt(1, sleep_isr, RISING);
  sleep_bod_disable();
  interrupts(); // enable interrupts i.e. sei();
  sleep_cpu();  // Put the device into sleep mode. The SE bit must be set beforehand, and it is recommended to clear it afterwards.

  /* wake up here */
  sleep_disable();
  //digitalWrite(pixPWRPin, LOW);  // turn on to begin LOW: ON, HIGH: OFF
  interrupts(); // end of some_condition
  Serial.println(F("I'm awake!"));
  init_vals();
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("### Piezo Flasher ###"));
  pinMode(A0, INPUT); // using A0 to control neopixel didn't work somehow...
  digitalWrite(A0, LOW); // make sure internal pullup is not set

  pinMode(piezoPin, INPUT);
  pinMode(pixPWRPin, OUTPUT);
  digitalWrite(pixPWRPin, LOW);  // turn on to begin
  randomSeed(analogRead(0));
  delay(10);
  adc_disable();  // turn off ADC circuit to reduce current consumption in sleep
  for (int i=0; i<2; i++){
    pinMode(modePins[i], INPUT_PULLUP);
  }
  strip.begin();
  strip.setBrightness(brightness);
  //strip.setPixelColor(0, 255,0,0);
  strip.show(); // Initialize all pixels to 'off'
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  Serial.println(F("Neopixel OFF"));
  digitalWrite(pixPWRPin, HIGH);  // cut power for neopixel
}

void loop()
{
  updateState();
  //state = PLAY_ONE; // enable this line to test different modes manually
  if (state == COIN_ONEUP){ // 0
    if (isBTNpressed()){
      Serial.println(F("  COIN_ONEUP"));
      Serial.print(F("counti: "));
      Serial.println(counti);
      if (counti > 9){
        play_a_song(MARIO_1UP);
        counti = 0;
        lightOFF();
      }
      else{
        lightCountUp();
        play_a_song(MARIO_COIN);
        counti++;
      }
    }
  }
  else if (state == PLAY_BY_NOTE){  // 3 = Default
    if (isBTNpressed()){
      Serial.println(F("  PLAY_BY_NOTE"));
      if (counti == 0){
        songi = random((int)TOTORO,(int)SHAKA); // TOTORO to SHAKA(excluding)
      }
      play_a_note(songi);
    }
  }
  else if (state == PLAY_BY_NOTE_SHAKA){
    if (isBTNpressed()){
      Serial.println(F("  PLAY_BY_NOTE"));
      play_a_note((int)SHAKA);
    }
  }
  else if (state == RACE_10SEC){ //
    if (isBTNpressed()){
      Serial.println(F("  RACE_10SEC"));
      Serial.print(F("counti: "));
      Serial.println(counti);
      lightCountUp();
      play_a_song(MARIO_COIN);
      counti++;
      if (millis() - gameStart > 10*1000){
        // race time passed
        if (counti > 30){
          // Champion!
          Serial.println(F("Champion!"));
          play_a_song(MARIO_COIN);
          play_a_song(MARIO_POWER_UP);
          play_a_song(MARIO_1UP);
        }
        else if (counti > 20){
          // Fair
          Serial.println(F("Fair"));
          play_a_song(MARIO_COIN);
        }
        else{
          // Lose!
          Serial.println(F("Lose!"));
          play_a_song(THE_END);
        }
        Serial.println(F("RESET"));
        Serial.println("");
        init_vals();
      }
    }
  }
  else if (state == PLAY_ONE_SEQUENCE){ //
    if (isBTNpressed()){
      Serial.println(F("  PLAY_ONE_SEQUENCE"));
      Serial.print(F("counti: "));
      Serial.println(counti);
      play_a_song(counti);
      if (counti < nsongs-1) counti++;
      else counti = 0;
    }
  }
  else if (state == ONEUP){ // 4
    // play 1UP sound every time button is pressed
    if (isBTNpressed()){
      Serial.println(F("  ONEUP"));
      flashOnce(255);
      play_a_song(MARIO_1UP);
    }
  }
  else if (state == COIN_ONEUP_RAMDOM){ //
    if (isBTNpressed()){
      Serial.println(F("  COIN_ONEUP_RAMDOM"));
      int songi = (int)random(10);
      Serial.print(F("songi: "));
      Serial.println(songi);
      if (songi == 0) play_a_song(MARIO_1UP);
      else play_a_song(MARIO_COIN);
    }
  }
  else if (state == CHIMES_RANDOM){ //
    if (isBTNpressed()){
      Serial.println(F("  CHIMES_RANDOM"));
      int i = (int)random(nchimes);
      Serial.print(F("i: "));
      Serial.println(i);
      play_a_song(chimes[i]);
    }
  }
  else if (state == PLAY_ONE_RANDOM){ //
    if (isBTNpressed()){
      Serial.println(F("  PLAY_ONE_RAMDOM"));
      int songi = (int)random(nsongs);
      Serial.print(F("songi: "));
      Serial.println(songi);
      play_a_song(songi);
    }
  }
  else if (state == PLAY_ALL){  //
    if (isBTNpressed()){
      Serial.println(F("  PLAY_ALL"));
      for (int i=0; i<nsongs; i++){
        play_a_song(i);
        delay(1000);
      }
    }
  }
  else if (state == PLAY_ONE){
    if (isBTNpressed()){
      Serial.println(F("  PLAY_ONE"));
      int songi = (int)YUME;
      Serial.print(F("songi: "));
      Serial.println(songi);
      play_a_song(songi);
    }
  }
  fadeout();
  if (millis() - lastPlayed > 5*1000) go_sleep();
}

void play_a_note(int s){
  pinMode(piezoPin, OUTPUT);
  int* song = songs[s];
  int* tempo = tempos[s];
  int len = lens[s];

  int size = len / sizeof(int);
  Serial.print(F("Playing "));
  Serial.print(counti);
  Serial.print(F("/"));
  Serial.println(size);
  flashOnce(len);
  _play_a_note(counti, song, tempo);
  pinMode(piezoPin, INPUT);
  lastPlayed = millis();

  if (counti > size -1) counti = 0;
  else counti++;
}

void play_a_song(int s){
  pinMode(piezoPin, OUTPUT);
  int* song = songs[s];
  int* tempo = tempos[s];
  int len = lens[s];

  brightness = 255;
  strip.setBrightness(brightness);
  strip.show();

  Serial.print("Play a song: ");
  Serial.println(s);
  int size = len / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    _play_a_note(thisNote, song, tempo);
    int noteDuration = 1000 / tempo[thisNote];
    int pauseBetweenNotes = noteDuration * 0.50;
    delay(pauseBetweenNotes);
  }
  pinMode(piezoPin, INPUT);
  lastPlayed = millis();
}

void _play_a_note(int thisNote, int* song, int* tempo){
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    //TODO: make 1000 variable to change speed
    int noteDuration = 1000 / tempo[thisNote];

    if (SOUND){
      tone(piezoPin, song[thisNote], noteDuration);
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      delay(noteDuration);
      // stop the tone playing:
      noTone(piezoPin);
    }
}

bool isBTNpressed(){
  int btn = digitalRead(piezoPin);
  //Serial.print("BTN: ");
  //Serial.println(btn);
  //if (!btn){
  if (btn){
    if (millis() - debounceTime > 100){
      // button is pressed
      debounceTime = millis();
      return true;
    }
  }
  return false;
}

void init_vals(){
    lightOFF();
    counti=0;
    gameStart=millis();
    lastPlayed = millis();
}

void updateState(){
  int selected_state =
          digitalRead(modePins[0]) +
          digitalRead(modePins[1]) * 2;
          //digitalRead(modePins[2]) * 4;
//  if (DEBUG) {
//    Serial.print(F("Selected State: "));
//    Serial.println(selected_state);
//  }
  // Defualt state is 3 because modePins are INPUT_PULLUP
  if (selected_state > nstates-1) selected_state = 3;
  state = (states)selected_state;
  if (prev_state != state){
    // entered new state
    init_vals();
    prev_state = state;
  }
}

void lightCountUp(){
  Serial.println(F("lightCountUp"));
  // counti, PIXEL_COUNT
  int maxval = min(counti, PIXEL_COUNT);
  int pixeli = counti % PIXEL_COUNT;
  int turns = counti / PIXEL_COUNT;
  int colori = turns % 3;
  float scale = (float)brightness / 255.0;
  uint32_t mycolor;
  if (DEBUG){
    Serial.print(F("turns: "));
    Serial.println(turns);
    Serial.print(F("colori: "));
    Serial.println(colori);
    Serial.print(F("scale: "));
    Serial.println(scale);
  }
  if (colori == 0){
    mycolor = strip.Color((int)255*scale, 0, 0);
  }
  else if (colori == 1){
    mycolor = strip.Color(0, (int)255*scale, 0);
  }
  else {
    mycolor = strip.Color(0, 0, (int)255*scale);
  }
  if (DEBUG){
    Serial.print(F("mycolor: "));
    Serial.println(mycolor);
    Serial.print(F("pixeli: "));
    Serial.println(pixeli);
  }
  strip.setPixelColor(pixeli, mycolor);
  //strip.setBrightness(brightness);
  strip.show();
  delay(10);
  strip.setPixelColor(pixeli, 0,0,0);
  //strip.setBrightness(brightness);
  strip.show();
}

void flashOnce(int len){
  Serial.println(F("flashOnce"));
  Serial.println(F("Neopixel ON"));
  digitalWrite(pixPWRPin, LOW);  // power on for neopixel
  lightOFF();
  brightness = 64;
  strip.setBrightness(brightness);
  //int mycount = counti % 255;
  int mycount = map(counti, 0, len, 0, 255);
  uint32_t mycolor = Wheel((byte)mycount);
  strip.setPixelColor(0, mycolor);
  if (PIXEL_COUNT > 1){
    int luckyi = (int)random(1,PIXEL_COUNT);
    strip.setPixelColor(luckyi, mycolor);
  }
  if (DEBUG){
    Serial.print(F("counti: "));
    Serial.println(counti);
    Serial.print(F("mycount: "));
    Serial.println(mycount);
    Serial.print(F("mycolor: "));
    Serial.println(mycolor);
  }
  strip.show();
//  delay(100);
//  strip.setPixelColor(0, 0,0,0);
//  if (PIXEL_COUNT > 1){
//    strip.setPixelColor(luckyi, 0,0,0);
//  }
//  strip.setBrightness(brightness);
//  strip.show();
}
void fadeout(){
  //Serial.print("Brightness: ");
  //Serial.println(brightness);
  if (brightness == 0){
    // pass
  }
  else if (brightness > DELTA_MINUS){
    brightness -= DELTA_MINUS;
    strip.setBrightness(brightness);
    strip.show();
  } else {
    brightness = 0;
    Serial.println(F("Neopixel OFF"));
    digitalWrite(pixPWRPin, HIGH);  // cut power for neopixel
  }
}

void turnKiraKiraOn(){
  kirakira = 1;
}
void turnKiraKiraOff(){
  kirakira = 0;
}

void lightOFF(){
  for (int i=0; i<PIXEL_COUNT; i++){
    strip.setPixelColor(i, 0,0,0);
  }
  strip.show();
}

//Theatre-style crawling lights.
// from Adafruit Neopixel example
void theaterChase(uint32_t c, uint8_t wait) {
  for (int q=0; q < 3; q++) {
    for (int i=0; i < strip.numPixels(); i=i+3) {
      strip.setPixelColor(i+q, c);    //turn every third pixel on
    }
    strip.show();

    delay(wait);

    for (int i=0; i < strip.numPixels(); i=i+3) {
      strip.setPixelColor(i+q, 0);        //turn every third pixel off
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
