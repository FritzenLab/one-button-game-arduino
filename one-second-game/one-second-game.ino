#define button 9

// ---------- Timing ----------
const unsigned long timeout        = 2000000UL;  // 2 s
const unsigned long roundCooldown  = 400000UL;   // 400 ms

// ---------- State ----------
bool lastButtonState   = HIGH;   // INPUT_PULLUP
bool gameRunning       = false;
bool readyForNextRound = true;
bool waitForRelease    = false;

// ---------- Time stamps ----------
unsigned long lastPressTime = 0;
unsigned long nextRoundTime = 0;

void setup() {
  pinMode(button, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  bool currentButtonState = digitalRead(button);
  unsigned long now = micros();

  // ---------- Wait until button is released ----------
  if (waitForRelease && currentButtonState == HIGH) {
    waitForRelease = false;
    readyForNextRound = false;   // cooldown must still expire
  }

  // ---------- Cooldown logic ----------
  // Waits a couple of miliseconds before initiating a new round
  if (!readyForNextRound &&
      !waitForRelease &&
      (now - nextRoundTime) >= roundCooldown) {
    readyForNextRound = true;
  }

  // ---------- Timeout ----------
  // This makes sure the game times out after 2 seconds of inactivity
  if (gameRunning && (now - lastPressTime) > timeout) {
    Serial.println("Game reset (timeout)");
    gameRunning = false;
    nextRoundTime = now;
    waitForRelease = true;
  }

  // ---------- Detect falling edge ----------
  // button was pressed and is not anymore
  if (lastButtonState == HIGH && currentButtonState == LOW) {

    // ----- First press: start round -----
    // Starts game
    if (!gameRunning && readyForNextRound) {
      gameRunning = true;
      lastPressTime = now;
      readyForNextRound = false;
      Serial.println("Start >>");
    }

    // ----- Second press: finish round -----
    // Here it is decided whether you won or lose after the second button press
    else if (gameRunning) {
      unsigned long delta = now - lastPressTime;

      // You only win if pressed the button between 0.95 and 1.05 seconds from the first press
      if (delta >= 950000UL && delta <= 1050000UL) {
        Serial.print("YOU WON: ");
        Serial.println(delta / 1000000.0);
      } // You lose if pressed the button between 0.5 and 0.95 seconds or over 1.05 seconds
      else if ((delta > 500000UL && delta < 950000UL) || delta > 1050000UL) {
        Serial.print("You lose: ");
        Serial.println(delta / 1000000.0);
      }
      // There is no prevision of pressing the button faster than 0.5 seconds
      // that prevents button bouncing from being detected as a new button press

      gameRunning = false;
      nextRoundTime = now;
      waitForRelease = true;
    }
  }

  lastButtonState = currentButtonState;
}
