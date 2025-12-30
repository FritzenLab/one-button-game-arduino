# One Button Arduino Game 🎮

A simple but surprisingly challenging **one-button game** implemented using an ESP32 and the Arduino framework.

The goal is to press a single push button **twice**, with the second press happening as close as possible to **exactly one second** after the first.

This project focuses on **timing accuracy**, **state machines**, and **non-blocking firmware design**, using only basic Arduino functions.

---

## 🕹️ Game Rules

- The **first button press** starts an internal timer (not shown).
- The **second press** must occur **exactly 1 second later**.
- You **win** if the second press happens between:
  - **0.95 s and 1.05 s** (±5% tolerance)
- You **lose** if:
  - The press is earlier than 0.95 s
  - The press is later than 1.05 s
- If no second press happens within **2 seconds**, the game resets.
- Presses faster than **0.5 seconds** are intentionally ignored to avoid bounce-related false triggers.
- After each round, there is a short **cooldown** before a new round can start.

Game output is shown via the **Serial Monitor**.

---

## 🧠 Design Philosophy

- **Single input** (one push button)
- **No `delay()`**
- Fully **non-blocking**
- Explicit **state tracking**
- Deterministic behavior

This project demonstrates how even simple games benefit from proper **state machines** and **time-based logic**.

---

## 🧰 Hardware Required

- ESP32 board (tested with **ESP32-C3 Super Mini**)
- 1× push button
- Breadboard and jumper wires
- USB cable (for power + serial)

> The ESP32 uses `INPUT_PULLUP`, so no external pull-up resistor is required.

---

## 🔌 Wiring

- One side of the push button → **GPIO 9**
- Other side → **GND**

---

## 🧪 How It Works (High Level)

The game cycles through well-defined states:

- **Idle** – waiting for the first press
- **Running** – timing between presses
- **Cooldown** – short delay before next round
- **Release lock** – waits for button release to avoid false restarts

Timing is measured using `micros()` for microsecond-level precision.

---
