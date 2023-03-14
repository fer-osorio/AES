#include"Cipher.hpp"
#include"OperationsGF256.hpp"
#include<iostream>
// -Prints an array of 4 bytes.
void printWord(const byte word[4]) {
	std::cout << '[';
	for(int i = 0; i < 4; i++) {
		if(word[i] < 16) std::cout << '0';
		printf("%X", word[i]);
		if(i != 3)
			std::cout << ", ";
	}
	std::cout << ']';
}

// -Prints an arrat of 16 bytes.
void printState(const byte state[16]) {
	byte i, j;
	for(i = 0; i < 4; i++) {
		std::cout << '[';
		for(j = 0; j < 4; j++) {
			if(state[4*j + i] < 16) std::cout << '0';
			printf("%X", state[4*j + i]);
			if(j != 3) std::cout << ", ";
		}
		std::cout << "]\n";
	}
}

// -Applies a substitution table (S-box) to each byte.
void SubBytes(byte state[16]) {
	for(int i = 0; i < 16; i++)
		state[i] = SBox[state[i]];
}

// -Shift rows of the state array by different offset.
void ShiftRows(byte state[16]) {
	int i, j;
	byte tmp[4];
	for(i = 1; i < 4; i++) {
		for(j = 0; j < 4; j++)
			tmp[j] = state[i + 4*j];
		for(j = 0; j < 4; j++)
			state[i + 4*j] = tmp[(j + i) % 4];
	}
}

// -Mixes the data within each column of the state array.
void MixColumns(byte state[16]) {
	byte a[] = {0x02, 0x03, 0x01, 0x01}, temp[4], i, j, k;
	for(i = 0; i < 4; i++) {
		for(k = 0; k < 4; k++)
			temp[k] = state[4*i + k];
		//Debugging porpuses.
		//printWord(temp);
		//std::cout << '\n';
		for(j = 0; j < 4; j++) {
			state[4*i + j] = multiply[ a[(4 - j) % 4] ][ temp[0] ];
			//if(i == 2)
			//	printf("%X * %X = %X, ", a[(4 - j) % 4], temp[0],  multiply[ a[(4 - j) % 4] ][ temp[0] ]);
			for(k = 1; k < 4; k++) {
				state[4*i + j] ^= multiply[ a[(k - j + 4) % 4] ][ temp[k] ];
				//if(i == 2)
				//	printf("%X * %X = %X, ", a[(k - j + 4) % 4], temp[k], multiply[ a[(k - j + 4) % 4] ][ temp[k] ]);
			}
			//if(i == 2)
			//std::cout << '\n';
		}
	}
}

// -Combines a round key with the state.
void AddRoundKey(byte state[16], byte w[], byte round) {
	for(int i = 0; i < 16; i++)
		state[i] ^= w[16*round + i];
}

// -Cipher routine.
void Cipher(byte in[16],byte w[], byte Nr) {
	byte i;
	std::cout << "\nInitial state:\n";
	printState(in);
	AddRoundKey(in, w, 0);
	std::cout << "\nRound key value: \n";
	printState(w);
	for(i = 1; i < Nr; i++) {
		std::cout << "\nStart of the round " << int(i) << ":\n" ;
		printState(in);
		SubBytes(in);
		std::cout << "\nAfter SubBytes:\n";
		printState(in);
		ShiftRows(in);
		std::cout << "\nAfter ShiftRows:\n";
		printState(in);
		MixColumns(in);
		std::cout << "\nAfter MixColumns:\n";
		printState(in);
		AddRoundKey(in, w, i);
		std::cout << "\nRound Key value:\n";
		printState(&(w[16*i]));
	}
	std::cout << "\nStart of the round " << int(i) << ":\n" ;
	SubBytes(in);
	std::cout << "\nAfter SubBytes:\n";
	printState(in);
	ShiftRows(in);
	std::cout << "\nAfter ShiftRows:\n";
	printState(in);
	AddRoundKey(in, w, i);
	std::cout << "\nRound Key value:\n";
	printState(&(w[16*i]));
}

