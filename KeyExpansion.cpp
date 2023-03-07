// -Key Expansion. Functions definitios.

#include<iostream>
#include"KeyExpansion.hpp"

void print(const byte word[4]) {
	std::cout << '[';
	for(int i = 0; i < 4; i++) {
		printf("%X", word[i]);
		if(i != 3)
			std::cout << ", ";
	}
	std::cout << ']';
}

// -Coping an array of 4 bytes.
void CopyWord(const byte word[4], byte copy[4]) {
	for(int i = 0; i < 4; i++)
		copy[i] = word[i];
}

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

// -XOR of arrays of 4 bytes.
void XORword(const byte w1[4], const byte w2[4], byte w[4]) {
	for(int i = 0; i < 4; i++)
		w[i] = w1[i] ^ w2[i];
}

// -Actual implementation of KeyExpansion routine.
void KeyExpansion(byte key[], byte Nk, byte w[]) {
	byte i = 0, Nr = Nk + 6, temp[4];
	// -The first Nk words are the key itself.
	for(; i < 4*Nk; i++)
		w[i] = key[i];
	for(i = Nk; i < 4*(Nr + 1); i++) {
		// -Guarding agains modify things that we
		//  don't want to modify.
		CopyWord(&(w[4*(i - 1)]), temp);
		std::cout << " temp = ";
		print(temp);
		// -i is a multiple of Nk.
		if(i % Nk == 0) {
			RotWord(temp);
			std::cout << " Rot = ";
			print(temp);
			SubWord(temp);
			std::cout << " RotSub = ";
			print(temp);
			XORword(temp, Rcon[i/Nk -1], temp);
			std::cout << " Rcon[i/Nk] = ";
			print(Rcon[i/Nk - 1]);
			std::cout << " RotSubXor = ";
			print(temp);
		}
		if(Nk == 8 && i % Nk == 4) {
			SubWord(temp);
		}
		XORword(&(w[4*(i - Nk)]), temp, &(w[4*i]));
		std::cout << '\n';
	}
}

