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
#define VERSION   "1.0"
#define MAX_CHARS 200

// Flags.
#define FL_ECHO    0
#define FL_CMDECHO 1

unsigned int cidx = 0;       // Current character index.
char rx_str[MAX_CHARS + 1];  // Accomodate the '\0'.
bool line_recv = false;      // Received a full line?
char command[MAX_CHARS + 1];
char argument[MAX_CHARS + 1];

int flags[] = {
  false,  // FL_ECHO
  true,   // FL_CMDECHO
};

/**
 * The classic Arduino setup routine.
 */
void setup() {
  // Initialize the serial port with a standard baud rate.
  Serial.begin(9600);

  // Splash text.
  Serial.print("Arduino COM Tester v");
  Serial.println(VERSION);
  Serial.println("Developed by Nathan Campos\r\n");

  // Print the command prompt.
  Serial.print("> ");
}

/**
 * The classic Arduino loop routine.
 */
void loop() {
  // Check if there's data available to be read.
  while (Serial.available()) {
    handle_rx();
  }

  // Received a full line, so let's parse it.
  if (line_recv) {
    parse_line();
  }
}

/**
 * Parses a line.
 */
void parse_line() {
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
    if (rx_str[0] != '\0') {
      split_line();
    }

    if (strcmp("ECHO", command) == 0) {
      // Echo mode.
      flags[FL_ECHO] = true;
      Serial.println("Echo mode enabled. To disable send ECHOOFF");
    } else if (strcmp("CMDECHO", command) == 0) {
      // Command echo.
      if (argument[0] == '1') {
        flags[FL_CMDECHO] = true;
        Serial.println("Command echo enabled.");
      } else if (argument[0] == '0') {
        flags[FL_CMDECHO] = false;
        Serial.println("Command echo disabled.");
      } else {
        Serial.println("Unknown argument.");
      }
    } else {
      // Unknown.
      Serial.print("Unknown command: ");
      Serial.println(command);
      Serial.print("Argument: ");
      Serial.println(argument);
    }
  }

  line_recv = false;

  if (flags[FL_CMDECHO]) {
    Serial.print("> ");
  }
}

/**
 * Handles the serial RX stuff.
 */
void handle_rx() {
  char c = (char)Serial.read();

  if (flags[FL_CMDECHO]) {
    Serial.print(c);
  }

  if (c == '\n') {
    // LF received.
    rx_str[cidx] = '\0';
    cidx = 0;
    line_recv = true;
  } else if (cidx == MAX_CHARS) {
    // Character limit exceeded.
    rx_str[cidx] = '\0';
    cidx = 0;
    line_recv = true;

    Serial.print("\r\nCommand buffer size exceeded. Buffer size is: ");
    Serial.println(MAX_CHARS);
  } else if (c == '\r') {
    // CR received.
  } else {
    // Any old character received.
    rx_str[cidx] = c;
    cidx++;
  }
}

/**
 * Splits the command and argument from the RX buffer line.
 */
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
      command[i] = toupper(c);
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
