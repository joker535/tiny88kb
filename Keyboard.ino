#include "DigiKeyboard.h"
#include "encoder.h"
Encoder ecc(13,14);

byte col_pins[] = { 8, 7, 6 };
byte row_pins[] = { 10, 11 };

byte code[] = {6,7,8,9,10,11};

/* key matrix size */
#define MATRIX_ROWS 2
#define MATRIX_COLS 3
typedef uint8_t matrix_row_t;
matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
typedef uint8_t pin_t;
#define MATRIX_ROW_SHIFTER ((matrix_row_t)1)

matrix_row_t matrix_prev[MATRIX_ROWS];

#define readPin(pin) (digitalRead(pin) == HIGH)

void onKey(int index){
  switch(index){
    case 0:
      DigiKeyboard.addKeyPress(KC_LCTRL);
      DigiKeyboard.addKeyPress(KC_UP);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
  }
}

void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        digitalWrite(row_pins[x],HIGH);
        pinMode(row_pins[x],INPUT);
    }
} 

void select_row(uint8_t row) {
  pinMode(row_pins[row],OUTPUT);
  digitalWrite(row_pins[row],LOW); 
}

void unselect_row(uint8_t row) { 
  digitalWrite(row_pins[row],HIGH);
        pinMode(row_pins[row],INPUT);}

void init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        pinMode(col_pins[x],INPUT_PULLUP);
    }
}

void blinkA(void){
  digitalWrite(0, HIGH);
  delay(100);               // wait for a second
  digitalWrite(0, LOW); 
}

 bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    // Select row
    select_row(current_row);
    delayMicroseconds(10); 

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        // Select the col pin to read (active low)
        uint8_t pin_state = readPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
    }

    // Unselect row
    unselect_row(current_row);
    delayMicroseconds(30); 

    // If the row has changed, store the row and return the changed flag.
    if (current_matrix[current_row] != current_row_value) {
        current_matrix[current_row] = current_row_value;
        return true;
    }
    return false;
}

uint8_t matrix_scan(void) {
  init_pins();
    bool changed = false;
    // Set row, read cols
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        changed |= read_cols_on_row(raw_matrix, current_row);
    }
    return (uint8_t)changed;
}

inline matrix_row_t matrix_get_row(uint8_t row) {

    return raw_matrix[row];

}

bool keyboard_task(void) {
    matrix_row_t        matrix_row    = 0;
    matrix_row_t        matrix_change = 0;
    bool changed = false;
    matrix_scan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        matrix_row    = matrix_get_row(r);
        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
            changed = true;
            matrix_row_t col_mask = 1;
            for (uint8_t c = 0; c < MATRIX_COLS; c++, col_mask <<= 1) {
                if (matrix_change & col_mask) {
                   if(matrix_row & col_mask){
//                     DigiKeyboard.addKeyPress(code[r*MATRIX_COLS + c]);
                       onKey(r*MATRIX_COLS + c);
                   }
                   

                    // record a processed key
                    matrix_prev[r] ^= col_mask;
                }
            }
        }
    }
    return changed;
}

void setup() {
  // don't need to set anything up to use DigiKeyboard
  init_pins();
  pinMode(0, OUTPUT);
  raw_matrix[0] = 0;
  raw_matrix[1] = 0;
}


void loop() {
  int8_t newv = ecc.encoderUpdate();
  
  if(newv > 0){
     DigiKeyboard.record();
     DigiKeyboard.addKeyPress(KC_LCTRL);
     DigiKeyboard.addKeyPress(KC_LEFT);
     DigiKeyboard.report();
     DigiKeyboard.record(); 
     DigiKeyboard.report();
  }else if(newv < 0){
     DigiKeyboard.record();
     DigiKeyboard.addKeyPress(KC_LCTRL);
     DigiKeyboard.addKeyPress(KC_RIGHT);
     DigiKeyboard.report();
     DigiKeyboard.record();
     DigiKeyboard.report();
  }
  DigiKeyboard.record();
  bool change = keyboard_task();
  if(change){
    DigiKeyboard.report();  
  }
  
  // It's better to use DigiKeyboard.delay() over the regular Arduino delay()
  // if doing keyboard stuff because it keeps talking to the computer to make
  // sure the computer knows the keyboard is alive and connected
  DigiKeyboard.delay(5);
}
