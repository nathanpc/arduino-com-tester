/**
 * Serial Port Tester
 *
 * The purpose of this sketch is to be a nice platform to
 * test any sort of serial port, be it of a computer, module,
 * or anything you fancy plugging this into.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#define MAX_CHARS 200

unsigned int cidx = 0;       // Current character index.
char rx_str[MAX_CHARS + 1];  // Accomodate the '\0'.
bool line_recv = false;      // Received a full line?

/**
 * The classic Arduino setup routine.
 */
void setup() {
  // Initialize the serial port with a standard baud rate.
  Serial.begin(9600);
}

/**
 * The classic Arduino loop routine.
 */
void loop() {
  if (line_recv) {
    // Simple echo test, let's keep things simple.
    Serial.print("< ");
    Serial.println(rx_str);

    line_recv = false;
  }
   serialEvent();
}

/**
 * This occurs whenever a new data comes in the hardware
 * serial RX. This routine is run between each time loop()
 * runs. Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    char c = (char)Serial.read();

    if (c == '\n') {
      // LF received.
      rx_str[cidx] = '\0';
      cidx = 0;
      line_recv = true;
    } else if (c == '\r') {
      // CR received.
    } else {
      // Any old character received.
      rx_str[cidx] = c;
      cidx++;
    }
  }
}

/* to change the baud rate:
 *  Serial.flush(); // wait for last transmitted data to be sent 
Serial.begin(newBaudRate);
while(Serial.available()) Serial.read(); // empty  out possible garbage from input buffer
// if the device was sending data while you changed the baud rate, the info in the input buffer
// is corrupted.
 */
