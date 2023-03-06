// -Key Expansion. Functions definitios.

#include"KeyExpansion.hpp"

// -Rotation of bytes to the left.
void RotWord(byte word[4]) {
	byte i = 0, buffer, j;
	// 'x & 0x03' is equivalent to x % 4.
	for(; i < 4; i++, j = i + 1 & 0x03) {
		buffer = word[i];
		word[i] = word[j];
		word[j] = buffer;
	}
}

// -Apply SBox to each byte of the word.
void SubWord(byte word[4]) {
	for(byte i = 0; i < 4; i++)
		word[i] = SBox[word[i]];
}

// -Actual implementation of KeyExpansion routine.
void KeyExpansion(byte key[], byte Nk, byte destination[], byte AESTYPE) {
	byte i = 0;
	for(; i < Nk; i++) {
		destination[4*i] = key[4*i];
		destination[4*i + 1] = key[4*i + 1];
		destination[4*i + 2] = key[4*i + 2];
		destination[4*i + 3] = key[4*i + 3];
	}
}



















