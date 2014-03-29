/*************************************************************************
  This is a port of the Arduino library for the Adafruit Thermal Printer
  to run on Linux systems like the Raspberry Pi or Beaglebone Black.
  Pick one up at --> http://www.adafruit.com/products/597
  These printers use TTL serial to communicate, 2 pins are required.

  Adafruit invests time and resources providing this open source code.
  Please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  Linux port by Tony DiCola (tony@tonydicola.com)
  MIT license, all text above must be included in any redistribution.
 *************************************************************************/

#ifndef Thermal_h
#define Thermal_h

#include <cctype>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

// Barcode types
#define UPC_A   0
#define UPC_E   1
#define EAN13   2
#define EAN8    3
#define CODE39  4
#define I25     5
#define CODEBAR 6
#define CODE93  7
#define CODE128 8
#define CODE11  9
#define MSI    10

class Adafruit_Thermal {

 public:

  Adafruit_Thermal(int fd);
  Adafruit_Thermal(const char* serial);
  ~Adafruit_Thermal();

  void
    begin(int heatTime=200),
    reset(),
    setDefault(),
    test(),
    testPage(),

    normal(),
    inverseOn(),
    inverseOff(),
    upsideDownOn(),
    upsideDownOff(),
    doubleHeightOn(),
    doubleHeightOff(),
    doubleWidthOn(),
    doubleWidthOff(),
    boldOn(),
    boldOff(),
    underlineOn(uint8_t weight=1),
    underlineOff(),
    strikeOn(),
    strikeOff(),

    justify(char value),
    feed(uint8_t x=1),
    feedRows(uint8_t),
    flush(),
    online(),
    offline(),
    sleepNow(),
    sleepAfter(uint8_t seconds),
    wake(),

    setSize(char value),
    setLineHeight(int val=32),

    printBarcode(char * text, uint8_t type),
    setBarcodeHeight(int val=50),

    printBitmap(int w, int h, const uint8_t *bitmap),

    timeoutSet(unsigned long),
    timeoutWait(),
    setTimes(unsigned long, unsigned long),

    setCharSpacing(int spacing), // Not working
    tab();                       // Not working

  //bool hasPaper();

  void writeChar(uint8_t c);

  void print(const char* str);
  void println(const char* str);

 protected:

  int _fd;
  uint8_t
    prevByte,      // Last character issued to printer
    column,        // Last horizontal column printed
    maxColumn,     // Page width (output 'wraps' at this point)
    charHeight,    // Height of characters, in 'dots'
    lineSpacing,   // Inter-line spacing (not line height), in dots
    barcodeHeight; // Barcode height in dots, not including text
  timeval 
    resumeTime;    // Wait until time exceeds this before sending byte
  unsigned long 
    dotPrintTime,  // Time to print a single dot line, in microseconds
    dotFeedTime;   // Time to feed a single dot line, in microseconds
  int
    printMode;
  void
    setPrintMode(uint8_t mask),
    unsetPrintMode(uint8_t mask),
    writePrintMode(),
    writeBytes(uint8_t a),
    writeBytes(uint8_t a, uint8_t b),
    writeBytes(uint8_t a, uint8_t b, uint8_t c),
    writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
};

#endif // Thermal_h
