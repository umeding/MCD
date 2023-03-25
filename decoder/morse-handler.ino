/*
 * Copyright (c) 2023 Uwe B. Meding -- All Rights Reserved
 */

typedef struct MorseLetterMap_s
{
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
char findLetter(char *morseCode)
{
  if (morseCode == NULL)
  {
    return ' ';
  }
  for (int i = 0; i < MAP_SIZE - 1; i++)
  {
    MorseLetter ml = morseLetterMap[i];
    if (strcmp(morseCode, ml.morseCode) == 0)
    {
      return ml.letter;
    }
  }
  return '*';
}

/**
 * @brief
 *
 * @param deltan
 * @return char
 */
char mapType(float deltan)
{
  if (deltan > 199.)
  {
    return 'W';
  }
  if (deltan <= 1.)
  {
    return '*';
  }
  if (deltan > 1. && deltan <= 4.)
  {
    return 'S';
  }
  if (deltan > 3. && deltan <= 10.)
  {
    return 'L';
  }
  return 'W';
}