#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// On an arduino UNO:       A4(SDA), A5(SCL).
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMBARS     4       // Number of bars to be displayed
#define RED_ZONE_LIMIT 100  // Threshold of redzone

#pragma once

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }    
}

void loop() {
  // get values from potentiometer to display them, input  to "drawBar" could be anything, but should be mapped 0-127
  int values[NUMBARS];
  for(int i = 0; i < NUMBARS; ++i){
    values[i] = map(analogRead(A0), 0, 1022, 0, 127);
  }
  drawBar(values);
}

void drawBar(int *values){

  display.clearDisplay();
  int counter = 0;

  for(int starting_point = 0; starting_point < SCREEN_HEIGHT; starting_point += SCREEN_HEIGHT / NUMBARS) {

    display.drawFastVLine(127, starting_point, SCREEN_HEIGHT / NUMBARS - 1, SSD1306_WHITE); // TOP LINE   

        // version 1 - lines instead of bar below redzone limit
    display.drawFastHLine(0, starting_point + SCREEN_HEIGHT / NUMBARS / 2 - 1, values[counter], SSD1306_WHITE); // LINE OF METER
    display.fillRect(values[counter] - 2, starting_point, 4, SCREEN_HEIGHT / NUMBARS - 1, SSD1306_WHITE); // TOPBAR OF METER

    if(values[counter] < RED_ZONE_LIMIT){ // BELOW REDZONE
      display.fillRect(RED_ZONE_LIMIT, starting_point, 2, SCREEN_HEIGHT / NUMBARS - 1, SSD1306_INVERSE); // Displaying red zone limit line if not achieved
      // display.fillRect(0, starting_point + 1, values[counter] - 8, SCREEN_HEIGHT / NUMBARS - 1, SSD1306_INVERSE); // Bar below red zone limit is empty
    }
    else{ // ABOVE REDZONE
      display.fillRect(RED_ZONE_LIMIT, starting_point + 1, values[counter] - RED_ZONE_LIMIT, SCREEN_HEIGHT / NUMBARS - 3, SSD1306_WHITE); // Bar above red zone limit is filled with color
    }

        // version 2 - rectangles all the way
    // display.fillRect(0, starting_point, values[counter], SCREEN_HEIGHT / NUMBARS - 1, SSD1306_WHITE); // METER BAR
    // if(values[counter] < RED_ZONE_LIMIT){
    //   display.fillRect(RED_ZONE_LIMIT, starting_point, 2, SCREEN_HEIGHT / NUMBARS - 1, SSD1306_INVERSE); // Displaying red zone limit line if not achieved
    //   display.fillRect(0, starting_point + 1, values[counter] - 8, SCREEN_HEIGHT / NUMBARS - 3, SSD1306_INVERSE); // Bar below red zone limit is empty
    // }
    // else{
    //   display.fillRect(0, starting_point + 1, RED_ZONE_LIMIT - 8, SCREEN_HEIGHT / NUMBARS - 3, SSD1306_INVERSE); // Bar above red zone limit is filled with color
    // }
    // display.drawFastVLine(0, starting_point, SCREEN_HEIGHT / NUMBARS - 1, SSD1306_WHITE); // BOTTOM LINE
    // counter++;
  }

  display.display();
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

