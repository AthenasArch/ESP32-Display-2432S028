// Adapted by Bodmer to work with a NodeMCU and ILI9341 or ST7735 display.
//
// This code currently does not "blink" the eye!
//
// Library used is here:
// https://github.com/Bodmer/M5Display
//
// To do, maybe, one day:
// 1. Get the eye to blink
// 2. Add another screen for another eye
// 3. Add varaible to set how wide open the eye is
// 4. Add a reflected highlight to the cornea
// 5. Add top eyelid shaddow to eye surface
// 6. Add aliasing to blur mask edge
//
// With one lidded eye drawn the code runs at 28-33fps (at 27-40MHz SPI clock)
// which is quite reasonable. Operation at an 80MHz SPI clock is possible but
// the display may not be able to cope with a clock rate that high and the
// performance improvement is small. Operate the ESP8266 at 160MHz for best
// frame rate. Note the images are stored in SPI FLASH (PROGMEM) so performance
// will be constrained by the increased memory access time.
//
// Original header for this sketch is below. Note: the technical aspects of the
// text no longer apply to this modified version of the sketch:

//--------------------------------------------------------------------------
// Uncanny eyes for PJRC Teensy 3.1 with Adafruit 1.5" OLED (product #1431)
// or 1.44" M5.Lcd LCD (#2088).  This uses Teensy-3.1-specific features and
// WILL NOT work on normal Arduino or other boards!  Use 72 MHz (Optimized)
// board speed -- OLED does not work at 96 MHz.
//
// Adafruit invests time and resources providing this open source code,
// please support Adafruit and open-source hardware by purchasing products
// from Adafruit!
//
// Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.
// MIT license.  SPI FIFO insight from Paul Stoffregen's ILI9341_t3 library.
// Inspired by David Boccabella's (Marcwolf) hybrid servo/OLED eye concept.
//--------------------------------------------------------------------------
//=====================================================================
// uncannyEyes ESP32_2432S028 : 2022.08.01 : macsbug
// https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
//=====================================================================
// HARD : EESP32_2432S028 : 
/// Dev environment  : Arduino IDE 1.8.19
//  Board Manager    : arduino-esp32 2.0.3-RC1
//  Board            : "ESP32 Dev Module"
//  Flash Size       : "4MB (32Mb)"
//  Partition Scheme : "No OTA (2MB APP/2MB SPIFSS)"
//  Pord             : "dev/cu.wchusbserial14240"
// Library           : lovyanGFX 
//                   : https://github.com/lovyan03/LovyanGFX
//=====================================================================
// M5Stack_uncannyEyes : 
//  org   : 2019.05.06 Transplant by macsbug
//  rev.1 : 2019.05.08 add SD_Updat : modify by macsbug
//  rev.2 : 2019.05.10 Left eye position and movement correction : macsbug
//  rev.3 : 2019.06.26 Correct the up and down movement of the pupil : macsbug
//  rev.4 : 2020.05.26 Wio Electronic Animated Eyes : Transplant by macsbug
//  rev.5 : 2021.07.08 WT32_S01 : Transplant by macsbug
//  rev.6 : 2022.03.10 WT32_S01 : Porting by macsbug
//  https://macsbug.wordpress.com/2022/03/10/lvgl-8-demo-with-wt32-sc01/
// HARD : WT32-SC01
//      : http://www.wireless-tag.com/portfolio/wt32-sc01/
//      : 3.5" 480x320 SPI ST7796S Display with Touch FT6236
//      : SCLK=14,MOSI=13,MISO=12,DC=21,CS=15,RST=22,BacKLight=23
//      : TOUCH_IRQ=39,Touch I2C SDA=18,Touch I2C SCL=19;
//------------------------------------------------------------------------
// Wio Electronic Animated Eyes :
//  https://macsbug.wordpress.com/2020/05/26/wio-electronic-animated-eyes/
// M5Stack Electronic Animated Eyes ;
//  https://macsbug.wordpress.com/2019/05/06/m5stack-electronic-animated-eyes/
// Bodmer/ESP8266_uncannyEyes/ESP8266_uncannyEyes.ino
//  https://github.com/Bodmer/ESP8266_uncannyEyes/blob/master/ESP8266_uncannyEyes.ino
// graphics : catEye.h,terminatorEye.h,doeEye.h,naugaEye.h,newtEye.h,owlEye.h
//  https://github.com/adafruit/Uncanny_Eyes/tree/master/uncannyEyes/graphics
// Electronic Animated Eyes using Teensy 3.1/3.2
//  https://learn.adafruit.com/animated-electronic-eyes-using-teensy-3-1?view=all
// adafruit/Uncanny_Eyes :
//  https://github.com/adafruit/Uncanny_Eyes/tree/master/uncannyEyes
//=====================================================================
// GPIO : 37 -> 2, 38 -> 5, 39 -> 12
//=====================================================================
// uncannuEyes with 16bit MRB3511 S3 Minikit : 2022.07.22 : macsbug
// https://macsbug.wordpress.com/2022/08/03/3-5%e2%80%b3-16bit-lcd-and-s3-minikit/
//=====================================================================
// HARD : ESP32 S3 MiniKit : Make an ESP32 S3 MiniKit
//  https://macsbug.wordpress.com/2022/08/01/make-an-esp32-s3-minikit/
//=====================================================================
 
#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h">
// Enable ONE of these #includes for the various eyes:
#include "defaultEye.h"     // Standard human-ish hazel eye
//#include "noScleraEye.h"  // Large iris, no sclera
//#include "dragonEye.h"    // Slit pupil fiery dragon// -0x001 to 0x007F
//#include "goatEye.h"      // Horizontal pupil goat  // -0x001 to 0x007F
//#include "catEye.h"       // #include <pgmspace.h>,][ to *,= to PROGMEM=
//#include "doeEye.h"       // #include <pgmspace.h>,][ to *,= to PROGMEM=
//#include "terminatorEye.h"// #include <pgmspace.h>,][ to *,= to PROGMEM=
//#include "naugaEye.h"     // #include <pgmspace.h>,][ to *,= to PROGMEM=
//#include "newtEye.h"      // #include <pgmspace.h>,][ to *,= to PROGMEM=
//#include "owlEye.h"       // #include <pgmspace.h>,][ to *,= to PROGMEM=
 
#define DISPLAY_DC      D3 // Data/command pin for BOTH displays
#define DISPLAY_RESET   D4 // Reset pin for BOTH displays
#define SELECT_L_PIN   27//38 // LEFT  eye chip select pin
#define SELECT_R_PIN   27//39 // RIGHT eye chip select pin
// INPUT CONFIG (for eye motion -- enable or comment out as needed) -------
// The ESP8266 is rather constrained here as it only has one analogue port.
// An I2C ADC could be used for more analogue channels
//#define JOYSTICK_X_PIN A0 // Analog pin for eye horiz pos (else auto)
//#define JOYSTICK_Y_PIN A0 // Analog pin for eye vert position (")
//#define JOYSTICK_X_FLIP   // If set, reverse stick X axis
//#define JOYSTICK_Y_FLIP   // If set, reverse stick Y axis
#define TRACKING          // If enabled, eyelid tracks pupil
#define IRIS_PIN       27//37 // Photocell or potentiometer (else auto iris)
#define IRIS_PIN_FLIP  27//37 // If set, reverse reading from dial/photocell
#define IRIS_SMOOTH       // If enabled, filter input from IRIS_PIN
#define IRIS_MIN      140 // Clip lower analogRead() range from IRIS_PIN
#define IRIS_MAX      260 // Clip upper "
#define WINK_L_PIN     27//39 // Pin for LEFT eye wink button
#define BLINK_PIN      27//38 // Pin for blink button (BOTH eyes)
#define WINK_R_PIN     27//37 // Pin for RIGHT eye wink button
#define AUTOBLINK         // If enabled, eyes blink autonomously
 
// Probably don't need to edit any config below this line, -----------------
// unless building a single-eye project (pendant, etc.), in which case one
// of the two elements in the eye[] array further down can be commented out.
// Eye blinks are a tiny 3-state machine.  Per-eye allows winks + blinks.
#define NOBLINK 0     // Not currently engaged in a blink
#define ENBLINK 1     // Eyelid is currently closing
#define DEBLINK 2     // Eyelid is currently opening
typedef struct {
  int8_t   pin;       // Optional button here for indiv. wink
  uint8_t  state;     // NOBLINK/ENBLINK/DEBLINK
  int32_t  duration;  // Duration of blink state (micros)
  uint32_t startTime; // Time (micros) of last state change
} eyeBlink;
 
struct {
  uint8_t     cs;     // Chip select pin
  eyeBlink    blink;  // Current blink state
} eye[] = { SELECT_L_PIN, { WINK_L_PIN, NOBLINK },
            SELECT_R_PIN, { WINK_R_PIN, NOBLINK } 
};
 
#define NUM_EYES 2    // pcs eye : 1 = 1 eye, 2 = 2 eye
uint32_t fstart = 0;  // start time to improve frame rate calculation at startup

boolean T_BLINK_PIN = false, T_WINK_L_PIN = false, T_WINK_R_PIN = false; 
boolean b_DOWN = false;

//========================================================================
// INITIALIZATION -- runs once at startup --------------------------------
void setup(void) {
  uint8_t e = 0;
  Serial.begin(115200);
  tft.init();
  tft.setBrightness(255);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  //pinMode(27,INPUT_PULLUP);
  fstart = millis()-1; // Subtract 1 to avoid divide by zero later
}
//======================================================================== 
// EYE-RENDERING FUNCTION --------------------------------------------------
#define BUFFER_SIZE 256 // 64 to 512 seems optimum = 30 fps for default eye
void drawEye( // Renders one eye.  Inputs must be pre-clipped & valid.
  // Use native 32 bit variables where possible as this is 10% faster!
  uint8_t  e,      // Eye array index; 0 or 1 for left/right：0/1 = left/right
  uint32_t iScale, // Scale factor for iris
  uint32_t scleraX,// First pixel X offset into sclera image：Center of right and left
  uint32_t scleraY,// First pixel Y offset into sclera image：Upper and lower centers
  uint32_t uT,     // Upper eyelid threshold value：Up and down
  uint32_t lT)     // Lower eyelid threshold value：Up and down
  {
  uint32_t screenX, screenY, scleraXsave;
  uint32_t screenX_; // add macsbug
  int32_t  irisX, irisY;
  uint32_t p, a;
  uint32_t d;
  uint32_t pixels = 0;
  uint16_t pbuffer[BUFFER_SIZE]; // This one needs to be 16 bit
  // Set up raw pixel dump to entire screen.  Although such writes can wrap
  // around automatically from end of rect back to beginning, the region is
  // reset on each frame here in case of an SPI glitch.
  if (e == 0){ tft.setAddrWindow (  1,0,128,128);} // set left  window
  if (e == 1){ tft.setAddrWindow (191,0,128,128);} // set right window
  // Now just issue raw 16-bit values for every pixel...
  scleraXsave = scleraX;
  irisY = scleraY - (SCLERA_HEIGHT - IRIS_HEIGHT ) / 2;  // add : rev.3 2019.06.26
  for(screenY=0; screenY<SCREEN_HEIGHT; screenY++, scleraY++, irisY++) {
    scleraX = scleraXsave;
    irisX = scleraXsave - (SCLERA_WIDTH - IRIS_WIDTH) / 2;
    for(screenX=0; screenX<SCREEN_WIDTH; screenX++, scleraX++, irisX++) {
      if (e == 1){screenX_ =                    screenX;}
      if (e == 0){screenX_ = SCREEN_WIDTH - 1 - screenX;}
      if((pgm_read_byte(lower+screenY * SCREEN_WIDTH + screenX_) <= lT) ||
         (pgm_read_byte(upper+screenY * SCREEN_WIDTH + screenX_) <= uT)){
        p = 0;  //Covered by eyelid
      } else if((irisY < 0) || (irisY >= IRIS_HEIGHT) ||
                (irisX < 0) || (irisX >= IRIS_WIDTH)) { // In sclera
        p = pgm_read_word(sclera + scleraY * SCLERA_WIDTH + scleraX);
      } else {                                          // Maybe iris...
        p = pgm_read_word(polar+irisY*IRIS_WIDTH+irisX);// Polar angle/dist
        d = (iScale * (p & 0x7F)) / 128;                // Distance (Y)
        if(d < IRIS_MAP_HEIGHT) {                       // Within iris area
          a = (IRIS_MAP_WIDTH * (p >> 7)) / 512;        // Angle (X)
          p = pgm_read_word(iris+d*IRIS_MAP_WIDTH+a);   // Pixel = iris
        } else {                                        // Not in iris
          p = pgm_read_word(sclera+scleraY*SCLERA_WIDTH+scleraX);//Pixel=clear
        }
      }
      *(pbuffer + pixels++) = p>>8 | p<<8;
      if (pixels >= BUFFER_SIZE){//yield();
        //tft.pushColors((uint8_t*)pbuffer,pixels*2); pixels=0; //drawEye  
          tft.pushColors(pbuffer,pixels); pixels=0; //drawEye    
      }
    }
  }
   if (pixels) { tft.pushColors(pbuffer, pixels); pixels = 0;}
}
//======================================================================== 
// EYE ANIMATION -----------------------------------------------------------
const uint8_t ease[] = { // Ease in/out curve for eye movements 3*t^2-2*t^3
    0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  3,   // T
    3,  3,  4,  4,  4,  5,  5,  6,  6,  7,  7,  8,  9,  9, 10, 10,   // h
   11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 22, 23,   // x
   24, 25, 26, 27, 27, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39,   // 2
   40, 41, 42, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 56, 57, 58,   // A
   60, 61, 62, 63, 65, 66, 67, 69, 70, 72, 73, 74, 76, 77, 78, 80,   // l
   81, 83, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 98,100,101,103,   // e
  104,106,107,109,110,112,113,115,116,118,119,121,122,124,125,127,   // c
  128,130,131,133,134,136,137,139,140,142,143,145,146,148,149,151,   // J
  152,154,155,157,158,159,161,162,164,165,167,168,170,171,172,174,   // a
  175,177,178,179,181,182,183,185,186,188,189,190,192,193,194,195,   // c
  197,198,199,201,202,203,204,205,207,208,209,210,211,213,214,215,   // o
  216,217,218,219,220,221,222,224,225,226,227,228,228,229,230,231,   // b
  232,233,234,235,236,237,237,238,239,240,240,241,242,243,243,244,   // s
  245,245,246,246,247,248,248,249,249,250,250,251,251,251,252,252,   // o
  252,253,253,253,254,254,254,254,254,255,255,255,255,255,255,255 }; // n
//======================================================================== 
#ifdef AUTOBLINK
uint32_t timeOfLastBlink = 0L, timeToNextBlink = 0L;
#endif
//======================================================================== 
void frame( // Process motion for a single frame of left or right eye
  uint32_t        iScale) {     // Iris scale (0-1023) passed in
  static uint32_t frames   = 0; // Used in frame rate calculation
  static uint8_t  eyeIndex = 0; // eye[] array counter
  int32_t         eyeX, eyeY;
  uint32_t        t = micros(); // Time at start of function
//Serial.print((++frames * 1000) / (millis() - fstart));
//Serial.println("fps");        // Show frame rate
  if(++eyeIndex >= NUM_EYES) eyeIndex = 0;// Cycle through eyes, 1 per call
  // Autonomous X/Y eye motion
  // Periodically initiates motion to a new random point, random speed,
  // holds there for random period until next motion.
  static boolean  eyeInMotion      = false;
  static int32_t  eyeOldX=512, eyeOldY=512, eyeNewX=512, eyeNewY=512;
  static uint32_t eyeMoveStartTime = 0L;
  static int32_t  eyeMoveDuration  = 0L;
  int32_t dt = t - eyeMoveStartTime;      // uS elapsed since last eye event
  if(eyeInMotion) {                       // Currently moving?
    if(dt >= eyeMoveDuration) {           // Time up?  Destination reached.
      eyeInMotion      = false;           // Stop moving
      eyeMoveDuration  = random(3000000L);// 0-3 sec stop
      eyeMoveStartTime = t;               // Save initial time of stop
      eyeX = eyeOldX = eyeNewX;           // Save position
      eyeY = eyeOldY = eyeNewY;
    } else { // Move time's not yet fully elapsed -- interpolate position
      int16_t e = ease[255 * dt / eyeMoveDuration] + 1;   // Ease curve
      eyeX = eyeOldX + (((eyeNewX - eyeOldX) * e) / 256); // Interp X
      eyeY = eyeOldY + (((eyeNewY - eyeOldY) * e) / 256); // and Y
    }
  } else {                                // Eye stopped
    eyeX = eyeOldX;
    eyeY = eyeOldY;
    if(dt > eyeMoveDuration) {            // Time up?  Begin new move.
      int16_t  dx, dy;
      uint32_t d;
      do {                                // Pick new dest in circle
        eyeNewX = random(1024);
        eyeNewY = random(1024);
        dx      = (eyeNewX * 2) - 1023;
        dy      = (eyeNewY * 2) - 1023;
      } while((d = (dx * dx + dy * dy)) > (1023 * 1023)); // Keep trying
      eyeMoveDuration  = random(50000,150000); // ~1/14sec
                       //random(72000,144000); // ~1/ 7sec
      eyeMoveStartTime = t;               // Save initial time of move
      eyeInMotion      = true;            // Start move on next frame
    }
  }
 
// Blinking
#ifdef AUTOBLINK
  // Similar to the autonomous eye movement above -- blink start times
  // and durations are random (within ranges).
  if((t - timeOfLastBlink) >= timeToNextBlink) { // Start new blink?
    timeOfLastBlink = t;
    uint32_t blinkDuration = random(36000, 72000); // ~1/28 - ~1/14 sec
    // Set up durations for both eyes (if not already winking)
    for(uint8_t e=0; e<NUM_EYES; e++) {
      if(eye[e].blink.state == NOBLINK) {
         eye[e].blink.state     = ENBLINK;
         eye[e].blink.startTime = t;
         eye[e].blink.duration  = blinkDuration;
      }
    }
    timeToNextBlink = blinkDuration * 3 + random(4000000);
  }
#endif
 
  if(eye[eyeIndex].blink.state) { // Eye currently blinking?
    // Check if current blink state time has elapsed
    if((t - eye[eyeIndex].blink.startTime) >= eye[eyeIndex].blink.duration){
      // Yes -- increment blink state, unless...
      if((eye[eyeIndex].blink.state == ENBLINK) && // Enblinking and...
      //((digitalRead(BLINK_PIN) == LOW) ||        // blink or wink held...
        ((T_BLINK_PIN) ||        // blink or wink held...
          //digitalRead(eye[eyeIndex].blink.pin) == LOW)) {
        (T_WINK_L_PIN)) ||
        (T_WINK_R_PIN))      
        {
            
        // Don't advance state yet -- eye is held closed instead
      } else { // No buttons, or other state...
        if(++eye[eyeIndex].blink.state > DEBLINK) {// Deblinking finished?
          eye[eyeIndex].blink.state = NOBLINK;     // No longer blinking
        } else { // Advancing from ENBLINK to DEBLINK mode
          eye[eyeIndex].blink.duration *= 2;//DEBLINK is 1/2 ENBLINK speed
          eye[eyeIndex].blink.startTime = t;
        }
      }
    }
  } else { // Not currently blinking...check buttons!
    if(T_BLINK_PIN) {
      // Manually-initiated blinks have random durations like auto-blink
      uint32_t blinkDuration = random(36000, 72000);
      for(uint8_t e=0; e<NUM_EYES; e++) {
        if(eye[e].blink.state    == NOBLINK) {
           eye[e].blink.state     = ENBLINK;
           eye[e].blink.startTime = t;
           eye[e].blink.duration  = blinkDuration;
        }
      }
  //} else if(digitalRead(eye[eyeIndex].blink.pin) == LOW) { // Wink!
    } else if((eyeIndex == 0 && T_WINK_L_PIN ) || (eyeIndex == 1 && T_WINK_R_PIN)) { // Wink!
      eye[eyeIndex].blink.state     = ENBLINK;
      eye[eyeIndex].blink.startTime = t;
      eye[eyeIndex].blink.duration  = random(45000, 90000);
    }
  }
 
  // Process motion, blinking and iris scale into renderable values
  // Iris scaling: remap from 0-1023 input to iris map height pixel units
  iScale = ((IRIS_MAP_HEIGHT + 1) * 1024) /
           (1024 - (iScale * (IRIS_MAP_HEIGHT - 1) / IRIS_MAP_HEIGHT));
 
  // Scale eye X/Y positions (0-1023) to pixel units used by drawEye()
  if (eyeIndex == 0){eyeX = map(eyeX,1023,0,0,SCLERA_WIDTH - 128);}// left
  if (eyeIndex == 1){eyeX = map(eyeX,0,1023,0,SCLERA_WIDTH - 128);}// right
  eyeY = map(eyeY, 0, 1023, 0, SCLERA_HEIGHT - 128);
  if(eyeIndex == 1) eyeX = (SCLERA_WIDTH - 128) - eyeX; // Mirrored display
 
  // Horizontal position is offset so that eyes are very slightly crossed
  // to appear fixated (converged) at a conversational distance.  Number
  // here was extracted from my posterior and not mathematically based.
  // I suppose one could get all clever with a range sensor, but for now...
  eyeX += 4;
  if(eyeX > (SCLERA_WIDTH - 128)) eyeX = (SCLERA_WIDTH - 128);
 
  // Eyelids are rendered using a brightness threshold image.  This same
  // map can be used to simplify another problem: making the upper eyelid
  // track the pupil (eyes tend to open only as much as needed -- e.g. look
  // down and the upper eyelid drops).  Just sample a point in the upper
  // lid map slightly above the pupil to determine the rendering threshold.
  static uint8_t uThreshold = 128;
  uint8_t        lThreshold, n;
 
#ifdef TRACKING
  int16_t sampleX = SCLERA_WIDTH  / 2 - (eyeX / 2), // Reduce X influence
          sampleY = SCLERA_HEIGHT / 2 - (eyeY + IRIS_HEIGHT / 4);
  // Eyelid is slightly asymmetrical, so two readings are taken, averaged
  if(sampleY < 0) n = 0;
  else n = (pgm_read_byte(upper + sampleY * SCREEN_WIDTH + sampleX) +
   pgm_read_byte(upper+sampleY*SCREEN_WIDTH+(SCREEN_WIDTH-1-sampleX)))/2;
  uThreshold = (uThreshold * 3 + n) / 4; // Filter/soften motion
  // Lower eyelid doesn't track the same way, but seems to be pulled upward
  // by tension from the upper lid.
  lThreshold = 254 - uThreshold;
#else // No tracking -- eyelids full open unless blink modifies them
  uThreshold = lThreshold = 0;
#endif
  // The upper/lower thresholds are then scaled relative to the current
  // blink position so that blinks work together with pupil tracking.
  if(eye[eyeIndex].blink.state) { // Eye currently blinking?
    uint32_t s = (t - eye[eyeIndex].blink.startTime);
    if(s >= eye[eyeIndex].blink.duration) s = 255;// At or past blink end
    else s = 255 * s / eye[eyeIndex].blink.duration; // Mid-blink
    s          = (eye[eyeIndex].blink.state == DEBLINK) ? 1 + s : 256 - s;
    n          = (uThreshold * s + 254 * (257 - s)) / 256;
    lThreshold = (lThreshold * s + 254 * (257 - s)) / 256;
  } else {
    n          = uThreshold;
  }
  // Pass all the derived values to the eye-rendering function:
  drawEye(eyeIndex, iScale, eyeX, eyeY, n, lThreshold);
}
//========================================================================
// AUTONOMOUS IRIS SCALING (if no photocell or dial) -----------------------
#if !defined(IRIS_PIN) || (IRIS_PIN < 0)
// Autonomous iris motion uses a fractal behavior to similate both the major
// reaction of the eye plus the continuous smaller adjustments that occur.
uint16_t oldIris = (IRIS_MIN + IRIS_MAX) / 2, newIris;
 
void split( // Subdivides motion path into two sub-paths w/randimization
  int16_t  startValue, // Iris scale value (IRIS_MIN to IRIS_MAX) at start
  int16_t  endValue,   // Iris scale value at end
  uint32_t startTime,  // micros() at start
  int32_t  duration,   // Start-to-end time, in microseconds
  int16_t  range) {    // Allowable scale value variance when subdividing
 
  if(range >= 8) {     // Limit subdvision count, because recursion
    range    /= 2;     // Split range & time in half for subdivision,
    duration /= 2;     // then pick random center point within range:
    int16_t  midValue = (startValue + endValue - range)/2+random(range);
    uint32_t midTime  = startTime + duration;
    split(startValue, midValue, startTime, duration, range);//First half
    split(midValue  , endValue, midTime  , duration, range);//Second half
  } else {             // No more subdivisons, do iris motion...
    int32_t dt;        // Time (micros) since start of motion
    int16_t v;         // Interim value
    while((dt = (micros() - startTime)) < duration) {
      v = startValue + (((endValue - startValue) * dt) / duration);
      if(v < IRIS_MIN)      v = IRIS_MIN; // Clip just in case
      else if(v > IRIS_MAX) v = IRIS_MAX;
      frame(v);        // Draw frame w/interim iris scale value
    }
  }
}
#endif // !IRIS_PIN
//======================================================================== 
// MAIN LOOP -- runs continuously after setup() ----------------------------
void loop() {
  KeyPadLoop();
#if defined(IRIS_PIN) && (IRIS_PIN >= 0)   // Interactive iris
  uint16_t v = 512; //analogRead(IRIS_PIN);// Raw dial/photocell reading
#ifdef IRIS_PIN_FLIP
  v = 1023 - v;
#endif
  v = map(v, 0, 1023, IRIS_MIN, IRIS_MAX); // Scale to iris range
#ifdef IRIS_SMOOTH // Filter input (gradual motion)
  static uint16_t irisValue = (IRIS_MIN + IRIS_MAX) / 2;
  irisValue = ((irisValue * 15) + v) / 16;
  frame(irisValue);
#else  // Unfiltered (immediate motion)
  frame(v);
#endif // IRIS_SMOOTH
#else  // Autonomous iris scaling -- invoke recursive function
  newIris = random(IRIS_MIN, IRIS_MAX);
  split(oldIris, newIris, micros(), 10000000L, IRIS_MAX - IRIS_MIN);
  oldIris = newIris;
#endif // IRIS_PIN
//screenshotToConsole();
  ClearKeys();
}
//========================================================================

//========================================================================
void ClearKeys(){
 b_DOWN=false;T_BLINK_PIN=false;T_WINK_L_PIN=false;T_WINK_R_PIN=false;
 }
//========================================================================
// BLINK_PIN
// eye[eyeIndex].blink.pin
// eye[] = { SELECT_L_PIN, { WINK_L_PIN, NOBLINK },
//          SELECT_R_PIN, { WINK_R_PIN, NOBLINK } 
bool KeyPadLoop(){
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  if (touched) {
    Serial.printf("X: %d, Y: %d\n", touchX, touchY);
    // DOWN --------------------------------------------------------------
    if(touchX <  80 && touchY < 160){
      ClearKeys(); b_DOWN  = true; delay( 25); return true;
    }
    // T_WINK_R_PIN -------------------------------------------------------------
    if(touchX > 320){
      ClearKeys(); T_WINK_R_PIN = true; delay(200); return true;
    }
    // T_WINK_L_PIN ----------------------------------------------------
    if(touchX < 160 ){
      ClearKeys(); T_WINK_L_PIN  = true; delay(250); return true;
    }
    // T_BLINK_PIN ------------------------------------------------------------
    if(touchX > 160 && touchX < 320){
      ClearKeys(); T_BLINK_PIN = true; delay(250); return true;
    }
    //--------------------------------------------------------------------
  }
  return false;
}
//========================================================================
