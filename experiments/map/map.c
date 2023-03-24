#include <stdio.h>
#include <string.h>

//#define NULL (char*)0

typedef struct MorseLetterMap_s {
  char *morseCode;
  char letter;
} MorseLetter;

MorseLetter morseLetterMap[] = {
  // Letters

  { ".-", 'A' },
  { "-...", 'B' },
  { "-.-.", 'C' },
  { "-..", 'D' },
  { ".", 'E' },
  { "..-.", 'F' },
  { "--.", 'G' },
  { "....", 'H' },
  { "..", 'I' },
  { ".---", 'J' },
  { "-.-", 'K' },
  { ".-..", 'L' },
  { "--", 'M' },
  { "-.", 'N' },
  { "---", 'O' },
  { ".--.", 'P' },
  { "--.-", 'Q' },
  { ".-.", 'R' },
  { "...", 'S' },
  { "-", 'T' },
  { "..-", 'U' },
  { "...-", 'V' },
  { ".--", 'W' },
  { "-..-", 'X' },
  { "-.--", 'Y' },
  { "--..", 'Z' },

  // Numbers 

  { ".----", '1' },
  { "..---", '2' },
  { "...--", '3' },
  { "....-", '4' },
  { ".....", '5' },
  { "-....", '6' },
  { "--...", '7' },
  { "---..", '8' },
  { "----.", '9' },
  { "-----", '0' },

  // Punctuation

  { "-.-.--", '!' },
  { "..--.", '!' },
  { ".-..-.", '"' },
  { "...-..-", '$' },
  { ".----.", '\'' },
  { "--..--", ',' },
  { "-....-", '-' },
  { ".-.-.-", '.' },
  { "-..-.", '/' },
  { "---...", ':' },
  { "-.-.-.", ';' },
  { "..--..", '?' },
  { ".--.-.", '@' },
  { "..--.-", '_' },
  { NULL, ' ' }
};

#define MAP_SIZE (sizeof(morseLetterMap) / sizeof(MorseLetter))
void printLetters() {
  MorseLetter *ml = morseLetterMap;
  while(ml->morseCode) {
    printf("%8s %c\n", ml->morseCode, ml->letter);
    ml = ++ml;
  }
}

char findLetter(char* morseCode) {
  if(morseCode == NULL) {
    return ' ';
  }
  for(int i=0; i<MAP_SIZE-1; i++) { 
    MorseLetter ml = morseLetterMap[i];
    if(strcmp(morseCode, ml.morseCode) == 0) {
      return ml.letter;
    }
  }
  return '*';
}

void printFind(char *morseCode) {
  char letter = findLetter(morseCode);
  printf("%8s <%c>\n", morseCode, letter);
}
int main(int ac, char **av) {
  printLetters();

  printFind(".-");
  printFind("--.-");
  printFind("-.-.--");
  printFind("-.--..---.");
  printFind(NULL);

}

