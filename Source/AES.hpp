#include<iostream>

#ifndef _INCLUDED_AES_
#define _INCLUDED_AES_

namespace AES {

struct Key;
std::ostream& operator << (std::ostream& ost, Key k);
struct Key {
	enum Length {																// -Allowed AES key lengths
		_128 = 128,
		_192 = 192,
		_256 = 256
	};
	enum OperationMode {
		ECB,																	// -Electronic Code Book (not recommended).
		CBC,																	// -Cipher Block Chaining.
		CFB,
		OFB,
		CTR,
		PVS																		// -PI xor and variable Sbox
	};
	private:
	OperationMode operation_mode;
	Length 	 length;															// -Length in bits.
	unsigned lengthBytes;														// -Length in bytes.
	char* key = NULL;
	char IV[16] = {0, 0, 0, 0,													// -Initial vector for the CBC operation mode.
				   0, 0, 0, 0,													// -This default value (just zeros) is left
				   0, 0, 0, 0,													//  for the case in which we do not use CBC.
				   0, 0, 0, 0};

	public:
	Key(const char* const _key, Length, OperationMode, const char* const _IV = NULL);
	Key(const Key&);
	Key(const char*const fname);												// -Building from binary file.
	~Key();

	Key& operator = (const Key&);
	friend std::ostream& operator << (std::ostream& ost, Key k);

	void set_OperationMode(OperationMode _operation_mode) {this->operation_mode = _operation_mode;}
	OperationMode getOperationMode() const{ return this->operation_mode; }
	void set_IV(const char*const _IV) {
		for(int i = 0; i < 16; i++) this->IV[i] = _IV[i];
	}
	void write_IV(char*const destination) const {								// -Writes IV in destination
		for(int i = 0; i < 16; i++) destination[i] = this->IV[i];				// -Warning: We are supposing we have at least 16 bytes of space in destination
	}
	void write_Key(char*const destination) const {
		for(unsigned i = 0; i < this->lengthBytes; i++) destination[i] = this->key[i];
	}
	unsigned getLengthBytes() const {return this->lengthBytes;}
	void save(const char* const) const;											// -Saving information in a binary file.
};

class Cipher;																	// -Declaring class name "Cipher". The intention is to use it in the next function
std::ostream& operator << (std::ostream& st, const Cipher& c);					// -Declaration here so this function is inside the namespace function.
class Cipher {
	Key key;
	int    Nk, Nr, keyExpLen;
	char*  keyExpansion = NULL;

	char a[4] 	 = {0x02, 0x03, 0x01, 0x01};									// For MixColumns.
	char aInv[4] = {0x0E, 0x0B, 0x0D, 0x09};									// For InvMixColumns.

	const char Rcon[10][4] = {													// -Notice that the value of the left most char in polynomial form is 2^i.
		{0x01, 0x00, 0x00, 0x00},
  		{0x02, 0x00, 0x00, 0x00},
  		{0x04, 0x00, 0x00, 0x00},
  		{0x08, 0x00, 0x00, 0x00},
  		{0x10, 0x00, 0x00, 0x00},
  		{0x20, 0x00, 0x00, 0x00},
  		{0x40, 0x00, 0x00, 0x00},
  		{(char)0x80, 0x00, 0x00, 0x00},
		{0x1B, 0x00, 0x00, 0x00},
  		{0x36, 0x00, 0x00, 0x00}
  	};

	unsigned char SBox[256] = {
		0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
		0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
		0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
		0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
		0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
		0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
		0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
		0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
		0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
		0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
		0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
		0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
		0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
		0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
		0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
		0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
	};

	unsigned char InvSBox[256] = {
		0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
		0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
		0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
		0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
		0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
		0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
		0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
		0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
		0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
		0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
		0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
		0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
		0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
		0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
		0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
	};

	bool usingDefaultSbox = true;												// -Tell us if the object is using the default Sbox (Specified in the NIST standard)
	void setSboxToDefauld();													// -If the Sbox is modified, returns it to the default value.
	void setSbox();																// -Sets Sbox using random numbers from the array PIroundKey. It supposes the
																				//	array has at least 256 elements

	char defaultKey[32] =
		{(char)0x60, (char)0x3D, (char)0xEB, (char)0x10,
         (char)0x15, (char)0xCA, (char)0x71, (char)0xBE,
         (char)0x2B, (char)0x73, (char)0xAE, (char)0xF0,
         (char)0x85, (char)0x7D, (char)0x77, (char)0x81,
         (char)0x1F, (char)0x35, (char)0x2C, (char)0x07,
         (char)0x3B, (char)0x61, (char)0x08, (char)0xD7,
         (char)0x2D, (char)0x98, (char)0x10, (char)0xA3,
         (char)0x09, (char)0x14, (char)0xDF, (char)0xF4};

	public:
	Cipher(const char* const _key, Key::Length len);
	Cipher(const Key&);
	Cipher(const Cipher& a);
	~Cipher();

	Cipher& operator = (const Cipher& a);
	friend std::ostream& operator << (std::ostream& st, const Cipher& c);

	void create_KeyExpansion(const char* const);								// -Creates key expansion
	void setIV(char IV[16]) const;												// -Sets the initial vector value. Required for the CBC operation mode

	void encryptECB(char*const data, unsigned size);							// -Encrypts the message pointed by 'data' using the ECB operation mode. The data
																				//	size (in bytes) is  provided by the 'size' argument.
	void decryptECB(char*const data, unsigned size)const;						// -Decrypts the message pointed by 'data' using the ECB operation mode. The data
																				//	size (in bytes) is  provided by the 'size' argument.

	void encryptCBC(char*const data, unsigned size);							// -Encrypts the message pointed by 'data' using the CBC operation mode. The data
																				//	size (in bytes) is  provided by the 'size' argument.
	void decryptCBC(char*const data, unsigned size)const;						// -Decrypts the message pointed by 'data'. The message must had been encrypted
																				//	using the CBC mode operation.
																				// -The size of the message is provided by the 'size' argument.
	void encryptPVS(char*const data, unsigned size);							// -Encrypts the message pointed by 'data' using the PI operation mode.
																				// -The data size (in bytes) is  provided by the 'size' argument.
	void decryptPVS(char*const data, unsigned size);							// -Decrypts the message pointed by 'data' using the PI operation mode.
																				// -The size of the message is provided by the 'size' argument.

	void decrypt(char*const data, unsigned size);

	void saveKey(const char*const fname) const {this->key.save(fname);}
	Key::OperationMode getOperationMode() const{ return this->key.getOperationMode(); }

	private:
	void XORblocks(char b1[16], char b2[16], char r[16]) const;					// -Xor operation over 16 bytes array.
	void printWord(const char word[4]);											// -Prints an array of 4 bytes.
	void printState(const char word[16]);										// -Prints an array of 16 bytes.
	void CopyWord(const char source[4], char destination[4]) const;				// -Coping an array of 4 bytes.
	void CopyBlock(const char source[16], char destination[16]) const;			// -Coping an array of 16 bytes.
	void XORword(const char w1[4], const char w2[4], char resDest[4]) const;	// -XOR of arrays of 4 bytes.
	void RotWord(char word[4]) const;											// -Rotation of bytes to the left.
	void SubWord(char word[4]) const;											// -Apply SBox to each char of the word.
	void SubBytes(char state[16]) const;										// -Applies a substitution table (S-box) to each char.
	void ShiftRows(char state[16]) const;										// -Shift rows of the state array by different offset.
	void MixColumns(char state[16]) const;										// -Mixes the data within each column of the state array.
	void AddRoundKey(char state[16], int round) const;							// -Combines a round key with the state.
	void InvSubBytes(char state[16]) const;										// -Applies the inverse substitution table (InvSBox) to each char.
	void InvShiftRows(char state[16]) const;									// -Inverse function of shift rows.
	void InvMixColumns(char state[16]) const;									// -Inverse function of MixColumns.
	void encryptBlock(char block[16]) const;									// -Encrypts an array of 16 bytes.
	void decryptBlock(char block[16]) const;									// -Decrypts an array of 16 bytes.
};
};
#endif