#include"AESOperations.hpp"

// -Applies a substitution table (S-box) to each byte.
void SubBytes(byte state[16]) {
	for(int i = 0; i < 16; i++)
		state[i] = Sbox[state[i]];
}

// -Shift rows of the state array by different offset.
void ShiftRows(byte state[16]) {
	int tmp, i, j;
	for(i = 1; i < 4; i++) {
		tmp = state[(i + 4*(i + 3)) % 16];
		for(j = 0; j < 3; j++) {
			state[(i + 4*j) % 16] = state[(i + 4*(j + i)) % 16];
		}
		state[(i + 4*j) % 16] = tmp;
	}
}

// -Mixes the data within each column of the state array.
void MixColumns(byte state[16]) {
	byte a = {0x02, 0x03, 0x01, 0x01}, i, j;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			state[4*i + j] ^= a[(j - i + 4) % 4];
}

// -Combines a round key with the state.
void AddRoundKey(byte state[16]; byte w[], byte round) {
	for(int i = 0; i < 16; i++)
		state[i] ^= w[4*round + i & 3];
}
