/**
   CTF code by Carlos Costa (SeekNDFPV @ youtube)
   Base RSSI checker code by Andrey Voroshkov (bshep)
   SPI driver based on fs_skyrf_58g-main.c by Simon Chambers
   fast ADC reading code is by "jmknapp" from Arduino forum
   fast port I/O code from http://masteringarduino.blogspot.com.by/2013/10/fastest-and-smallest-digitalread-and.html
   

  The MIT License (MIT)

  Copyright (c) 2018 by Carlos Costa (SeekND FPV) & Ziege-One

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

// For saving settings in EEPROM
/*
 !WARNING!
 Writing takes 3.3ms.
 Maximum life of the EEPROM is 100000 writings/readings.
 Be careful not to use it too much, it is not replacable!
 */
#define adr_eprom_test 0                 // For the test for 1st time init of the Arduino (First power on)
#define adr_eprom_rssiThreshold 2        // (CC) play around with this value to find your best RSSI threshold. (between 190 to 260 seem to be acceptable values). The higher the value the closer the quad needs to get. But if it's too high it wont detect it.

#define adr_eprom_pilot1 4               // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot2 6               // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot3 8               // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot4 10              // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot5 12              // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot6 14              // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot7 16              // CTF colours 1 = blue , 2= red
#define adr_eprom_pilot8 18              // CTF colours 1 = blue , 2= red

#define adr_eprom_pilot1o 20             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot2o 22             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot3o 24             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot4o 26             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot5o 28             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot6o 30             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot7o 32             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange
#define adr_eprom_pilot8o 34             // all colours for other - non team - game times 1 = blue , 2= red , 3= green, 4= yellow, 5= purple, 6= cyan, 7= white, 8= orange

#define adr_eprom_numberPilots 36        // numberPilots 1-8

#define adr_eprom_BRIGHTNESS 38          // BRIGHTNESS
#define adr_eprom_haveexternalleds 40    //
#define adr_eprom_internalLeds 42        // max 20 only with external voltage // max 10 with 5v from Arduino
#define adr_eprom_externalLeds 44        // max 20 total with internal and external leds (eg: if you are using 10 internal leds you can use 10 max external

#define adr_eprom_mode 46                // set your prefered mode
#define adr_eprom_spLevel 48             // set the level of difficulty you want for the single player CTF only
#define adr_eprom_timerMin 50            // GAME TIMER minutes
#define adr_eprom_timerSec 52            // GAME TIMER seconds


uint16_t read_eprom(int address){
  return  (uint16_t) EEPROM.read(address) * 256 + EEPROM.read(address+1) ;
}

void write_eprom(int address,uint16_t val){
  EEPROM.write(address, val  / 256);
  EEPROM.write(address+1,val % 256 );
}

void writeEprom(){
  
    write_eprom(adr_eprom_rssiThreshold,rssiThreshold);

    write_eprom(adr_eprom_pilot1,pilot1);
    write_eprom(adr_eprom_pilot2,pilot2);
    write_eprom(adr_eprom_pilot3,pilot3);
    write_eprom(adr_eprom_pilot4,pilot4);
    write_eprom(adr_eprom_pilot5,pilot5);
    write_eprom(adr_eprom_pilot6,pilot6);
    write_eprom(adr_eprom_pilot7,pilot7);
    write_eprom(adr_eprom_pilot8,pilot8);

    write_eprom(adr_eprom_pilot1o,pilot1o);
    write_eprom(adr_eprom_pilot2o,pilot2o);
    write_eprom(adr_eprom_pilot3o,pilot3o);
    write_eprom(adr_eprom_pilot4o,pilot4o);
    write_eprom(adr_eprom_pilot5o,pilot5o);
    write_eprom(adr_eprom_pilot6o,pilot6o);
    write_eprom(adr_eprom_pilot7o,pilot7o);
    write_eprom(adr_eprom_pilot8o,pilot8o);

    write_eprom(adr_eprom_numberPilots,numberPilots);

    write_eprom(adr_eprom_BRIGHTNESS,BRIGHTNESS);
    write_eprom(adr_eprom_haveexternalleds,haveexternalleds);
    write_eprom(adr_eprom_internalLeds,internalLeds);
    write_eprom(adr_eprom_externalLeds,externalLeds);

    write_eprom(adr_eprom_mode,mode);
    write_eprom(adr_eprom_spLevel,spLevel);
    write_eprom(adr_eprom_timerMin,timerMin);
    write_eprom(adr_eprom_timerSec,timerSec);
}

void initEprom(){
  // Test for 1st time init of the Arduino (First power on)
  int test = read_eprom(adr_eprom_test);
  if (test != 123)
  {
    write_eprom(adr_eprom_test,123);
    writeEprom();
  }
  // Read saved values from EEPROM
  
  rssiThreshold = read_eprom(adr_eprom_rssiThreshold);

  pilot1 = read_eprom(adr_eprom_pilot1);
  pilot2 = read_eprom(adr_eprom_pilot2);
  pilot3 = read_eprom(adr_eprom_pilot3);
  pilot4 = read_eprom(adr_eprom_pilot4);
  pilot5 = read_eprom(adr_eprom_pilot5);
  pilot6 = read_eprom(adr_eprom_pilot6);
  pilot7 = read_eprom(adr_eprom_pilot7);
  pilot8 = read_eprom(adr_eprom_pilot8);

  pilot1o = read_eprom(adr_eprom_pilot1o);
  pilot2o = read_eprom(adr_eprom_pilot2o);
  pilot3o = read_eprom(adr_eprom_pilot3o);
  pilot4o = read_eprom(adr_eprom_pilot4o);
  pilot5o = read_eprom(adr_eprom_pilot5o);
  pilot6o = read_eprom(adr_eprom_pilot6o);
  pilot7o = read_eprom(adr_eprom_pilot7o);
  pilot8o = read_eprom(adr_eprom_pilot8o);

  numberPilots = read_eprom(adr_eprom_numberPilots);

  BRIGHTNESS = read_eprom(adr_eprom_BRIGHTNESS);
  haveexternalleds = read_eprom(adr_eprom_haveexternalleds);
  internalLeds = read_eprom(adr_eprom_internalLeds);
  externalLeds = read_eprom(adr_eprom_externalLeds);

  mode = read_eprom(adr_eprom_mode);
  spLevel = read_eprom(adr_eprom_spLevel);
  timerMin = read_eprom(adr_eprom_timerMin);
  timerSec = read_eprom(adr_eprom_timerSec);
}
