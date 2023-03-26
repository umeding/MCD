/*
 * Copyright (c) 2023 Uwe B. Meding -- All Rights Reserved
 *
 * 2023-03-26 uwe <uwe@uwemeding.com> -- created
 *  - detect the morse code symbols and letters.
 */

typedef struct MorseLetterMap_s {
  char *morseCode;
  char letter;
} MorseLetter;

MorseLetter morseLetterMap[] = {
    // Letters

    {".-", 'A'},
    {"-...", 'B'},
    {"-.-.", 'C'},
    {"-..", 'D'},
    {".", 'E'},
    {"..-.", 'F'},
    {"--.", 'G'},
    {"....", 'H'},
    {"..", 'I'},
    {".---", 'J'},
    {"-.-", 'K'},
    {".-..", 'L'},
    {"--", 'M'},
    {"-.", 'N'},
    {"---", 'O'},
    {".--.", 'P'},
    {"--.-", 'Q'},
    {".-.", 'R'},
    {"...", 'S'},
    {"-", 'T'},
    {"..-", 'U'},
    {"...-", 'V'},
    {".--", 'W'},
    {"-..-", 'X'},
    {"-.--", 'Y'},
    {"--..", 'Z'},

    // Numbers

    {".----", '1'},
    {"..---", '2'},
    {"...--", '3'},
    {"....-", '4'},
    {".....", '5'},
    {"-....", '6'},
    {"--...", '7'},
    {"---..", '8'},
    {"----.", '9'},
    {"-----", '0'},

    // Punctuation

    {"-.-.--", '!'},
    {"..--.", '!'},
    {"-.--.", '('},
    {"-.--.-", ')'},
    {".-..-.", '"'},
    {"...-..-", '$'},
    {".----.", '\''},
    {"--..--", ','},
    {"-....-", '-'},
    {".-.-.-", '.'},
    {"-..-.", '/'},
    {"---...", ':'},
    {"-.-.-.", ';'},
    {"..--..", '?'},
    {".--.-.", '@'},
    {"..--.-", '_'},
    {NULL, ' '}};

#define MAP_SIZE (sizeof(morseLetterMap) / sizeof(MorseLetter))

/**
 * Find the letter in the Morse code map.
 *
 * @param morseCode the Morse code
 * @return char  the corresponding character
 */
char findLetter(char *morseCode) {
  if (morseCode == NULL) {
    return ' ';
  }
  for (int i = 0; i < MAP_SIZE - 1; i++) {
    MorseLetter ml = morseLetterMap[i];
    if (strcmp(morseCode, ml.morseCode) == 0) {
      return ml.letter;
    }
  }
  return '*';
}

volatile int morsecodeSymbolLen = 0;

#define MAX_CAPTURE_LEN 10
volatile char morsecodeCaptureBuffer[MAX_CAPTURE_LEN];
volatile char *morsecodeStart = morsecodeCaptureBuffer;
volatile char *morsecodeEnd = morsecodeStart + MAX_CAPTURE_LEN;

/**
 * Reset the current letter detection.
 */
void resetDetection(int startValue) {
  morsecodeSymbolLen = startValue;
  memset(morsecodeCaptureBuffer, 0, sizeof(morsecodeCaptureBuffer));
  morsecodeStart = morsecodeCaptureBuffer;
}

void handleSymbolCont() {
  morsecodeSymbolLen++;
}

/**
 * Handle a symbol, either a dit or a dah.
 */
void handleSymbol() {
  // avoid buffer overflow
  if (morsecodeStart >= morsecodeEnd) {
    return;
  }

  char symbol = morsecodeSymbolLen >= 3 ? '-' : '.';
  *morsecodeStart++ = symbol;

  // Serial.print(">>> ");
  // Serial.print(morsecodeSymbolLen);
  // Serial.print(" ");
  // Serial.println(symbol);

  morsecodeSymbolLen = 1;
}

/**
 * Handle a letter, meaning what we captured
 * so far should be a letter.
 */
void handleLetter() {
  // peel out the letter...
  handleSymbol();
  printMorseCode();

  // reset for the next letter
  resetDetection(1);
}

/**
 * Handle, meaning flush the final stuff we captured and have not
 * processed so far.
 */
void handleFinalLetter() {
  // peel out the letter...
  handleSymbol();
  printMorseCode();

  // reset for the next letter
  resetDetection(0);
}

/**
 * We recognized a space.
 */
void handleSpace() {
  printMorseSpace();

  // reset for the next letter
  resetDetection(1);
}

void printMorseCode() {
  char letter = findLetter(morsecodeCaptureBuffer);

#if MORSE_PRINT_CODE
  char *pMC = morsecodeCaptureBuffer;
  Serial.print(">>> ");
  Serial.print(pMC);
  Serial.print(" ==> ");
  Serial.println(letter);
#endif

#if MORSE_PRINT_LETTER
  Serial.print(letter);
#endif
}

void printMorseSpace() {

#if MORSE_PRINT_CODE
  Serial.println(">>>  (space)");
#endif

#if MORSE_PRINT_LETTER
  Serial.print(' ');
#endif
}

/**
 * Detect the timing based on the normalized
 * loop timing.
 *
 * @param deltan normalized timing
 * @return char timing indictor
 */
inline char detectTiming(float deltan) {
  if (deltan <= 1.) {
    // symbol continuation
    return MORSE_SYMBOL_CONT;
  }
  if (deltan > 1. && deltan <= 3.) {
    // symbol finished
    return MORSE_SYMBOL;
  }
  if (deltan > 3. && deltan <= 10.) {
    // letter finished
    return MORSE_LETTER;
  }
  // space between words
  return MORSE_WORD_SPACE;
}

/**
 * Handle a beep.
 *
 * @param delta time difference to last beep
 * @param mag beep magnitude
 */
void morseBeep(float delta, float mag) {
  float deltaNormalized = NORMALIZE_TIMING(delta);
  char type = detectTiming(deltaNormalized);

#if MORSE_PRINT_TIMING
  // Serial.println("======");
  Serial.print(type);
  Serial.print(" ");
  Serial.print(deltaNormalized);
  Serial.print(" ");
  Serial.print(delta);
  Serial.print(" ");
  Serial.println(mag);
#endif

  switch (type) {
    case MORSE_SYMBOL:
      handleSymbol();
      break;

    case MORSE_SYMBOL_CONT:
      handleSymbolCont();
      break;

    case MORSE_LETTER:
      handleLetter();
      break;

    default:
    case MORSE_WORD_SPACE:
      handleSpace();
      break;
  }
}

/**
 * Handle the remaining stuff and then flush
 * the buffers.
 */
void morseFlush() {
  if (morsecodeSymbolLen == 0)
    return;
  handleFinalLetter();
}