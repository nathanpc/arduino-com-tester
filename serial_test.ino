/**
 * Serial Port Tester
 *
 * The purpose of this sketch is to be a nice platform to
 * test any sort of serial port, be it of a computer, module,
 * or anything you fancy plugging this into.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

// Constants stuff.
#define MAX_CHARS 200

// Flags.
#define FL_ECHO 0

unsigned int cidx = 0;       // Current character index.
char rx_str[MAX_CHARS + 1];  // Accomodate the '\0'.
bool line_recv = false;      // Received a full line?
char command[MAX_CHARS + 1];
char argument[MAX_CHARS + 1];

int flags[] = {
  false  // FL_ECHO
};

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
  // Check if there's data available to be read.
  while (Serial.available()) {
    char c = (char)Serial.read();

    // TODO: Prevent cidx from passing MAX_CHARS-1.

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

  // Received a full line, so let's parse it.
  if (line_recv) {
    if (flags[FL_ECHO]) {
      // Echo mode.
      if (strcmp(rx_str, "ECHOOFF") == 0) {
        // Disable echo mode.
        flags[FL_ECHO] = false;
        Serial.println("Echo mode disabled.");
      } else {
        Serial.print("< ");
        Serial.println(rx_str);
      }
    } else {
      // Command mode.
      split_line();

      if (strcmp("ECHO", command) == 0) {
        flags[FL_ECHO] = true;
        Serial.println("Echo mode enabled. To disable send ECHOOFF");
      } else {
        Serial.print("Unknown command: ");
        Serial.println(command);
        Serial.print("Argument: ");
        Serial.println(argument);
      }
    }

    line_recv = false;
  }
}

void split_line() {
  char c;

  for (unsigned int i = 0; i < MAX_CHARS; i++) {
    c = rx_str[i];

    if (c == ' ') {
      // Found a token.
      command[i] = '\0';

      // Get the argument.
      for (unsigned int j = i + 1; j < MAX_CHARS; j++) {
        c = rx_str[j];
        argument[j - i - 1] = c;

        if (c == '\0') {
          // Found the end of the string, let's stop.
          break;
        }
      }

      break;
    } else if (c == '\0') {
      // Found the end of the string.
      command[i] = '\0';
      argument[0] = '\0';
      break;
    } else {
      // Found a normal character.
      command[i] = c;
      // TODO: Auto uppercase?
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
