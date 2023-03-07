// KeyExpansion routine headers.
#include"AESOperations.hpp"

// -Notice that the value of the left most byte 
//  in polynomial form is Rcon[i] == 2^i.
const byte Rcon[][4] =
{ {0x01, 0x00, 0x00, 0x00},
  {0x02, 0x00, 0x00, 0x00},
  {0x04, 0x00, 0x00, 0x00},
  {0x08, 0x00, 0x00, 0x00},
  {0x10, 0x00, 0x00, 0x00},
  {0x20, 0x00, 0x00, 0x00},
  {0x40, 0x00, 0x00, 0x00},
  {0x80, 0x00, 0x00, 0x00},
  {0x1B, 0x00, 0x00, 0x00},
  {0x36, 0x00, 0x00, 0x00} };

void print(const byte word[4]);

// -Coping an array of 4 bytes.
void CopyWord(const byte word[4], byte copy[4]);

// -XOR of arrays of 4 bytes.
void XORword(const byte w1[4], const byte w2[4], byte w[4]);

// -Rotation of bytes to the left.
void RotWord(byte word[4]);

// -Apply SBox to each byte of the word.
void SubWord(byte word[4]);

// -Actual implementation of KeyExpansion routine.
void KeyExpansion(byte key[], byte Nk,  byte w[]);
