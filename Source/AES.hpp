#include<iostream>

#ifndef _INCLUDED_AES_
#define _INCLUDED_AES_
#define AES_BLK_SZ 16

namespace AES {

struct Key;
std::ostream& operator << (std::ostream& ost, Key k);
struct Key {
	enum Length {_128 = 128,_192 = 192,_256 = 256};								// -Allowed AES key lengths
	enum OperationMode {
		ECB,																	// -Electronic Code Book (not recommended).
		CBC,																	// -Cipher Block Chaining.
		CFB,
		OFB,
		CTR,
	};
	private:
	char*	 key = NULL;
	Length 	 lengthBits;														// -Length in bits.
	unsigned lengthBytes;														// -Length in bytes.
	mutable  OperationMode operation_mode;
	mutable  bool notSetUpIV = true;											// -Tells if the initial vector is already initialized or not
	mutable  char IV[AES_BLK_SZ] = {0, 0, 0, 0,									// -Initial vector for the CBC operation mode
									0, 0, 0, 0,									// -This default value (just zeros) is left
				    				0, 0, 0, 0,									//  for the case in which we do not use CBC
				    				0, 0, 0, 0};

	public:
	Key();																		// -Assigns lengthBits of 256 bits and zero value for each byte of array char* key
	Key(const char* const _key, Length, OperationMode, const char* const _IV = NULL);
	Key(const char*const fname);												// -Building from binary file.
	Key(const Key&);
	~Key();

	Key& operator =  (const Key&);
	bool operator == (const Key&) const;
	friend std::ostream& operator << (std::ostream& ost, Key k);

	void set_OperationMode(OperationMode _operation_mode) const{ this->operation_mode = _operation_mode; }
	OperationMode getOperationMode() const{ return this->operation_mode; }
	void set_IV(const char*const _IV) const{									// -Initializing initial vector with the values pointed by _IV pointer. Warning:
		for(int i = 0; i < AES_BLK_SZ; i++) this->IV[i] = _IV[i];				//	We're supposing _IV points to an array of at least 16 elements
		this->notSetUpIV = false;
	}
	bool IVisNotSetUp() const { return this->notSetUpIV; }
	void write_IV(char*const destination) const {								// -Writes IV in destination
		for(int i = 0; i < AES_BLK_SZ; i++) destination[i] = this->IV[i];// -Warning: We are supposing we have at least 16 bytes of space in destination
	}
	void write_Key(char*const destination) const {								// -Writes key in destination. Warning: We're supposing we have enough space in
		for(unsigned i = 0; i < this->lengthBytes; i++) destination[i] = this->key[i];	//	destination array.
	}
	unsigned getLengthBytes() const {return this->lengthBytes;}
	void save(const char* const) const;											// -Saving information in a binary file.
};

class Cipher;																	// -Declaring class name "Cipher". The intention is to use it in the next function
std::ostream& operator << (std::ostream& st, const Cipher& c);					// -Declaration here so this function is inside the namespace function.
class Cipher {
	Key		key = Key();														// -The default values for a cipher object are the values for a key of 256 bits
	int		Nk = 8, Nr = 14, keyExpLen = 240;
	char*	keyExpansion = NULL;

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

	public:
	Cipher();																	// -The default constructor will set the key expansion as zero in every element.
	Cipher(const Key&);
	Cipher(const Cipher& a);
	~Cipher();

	Cipher& operator = (const Cipher& a);
	friend std::ostream& operator << (std::ostream& st, const Cipher& c);

	void create_KeyExpansion(const char* const);								// -Creates key expansion
	void setIV(char IV[AES_BLK_SZ]) const;										// -Sets the initial vector value. Required for the CBC operation mode

	void encryptECB(char*const data, unsigned size)const;						// -Encrypts the message pointed by 'data' using the ECB operation mode. The data
																				//	size (in bytes) is  provided by the 'size' argument.
	void decryptECB(char*const data, unsigned size)const;						// -Decrypts the message pointed by 'data' using the ECB operation mode. The data
																				//	size (in bytes) is  provided by the 'size' argument.

	void encryptCBC(char*const data, unsigned size)const;						// -Encrypts the message pointed by 'data' using the CBC operation mode. The data
																				//	size (in bytes) is  provided by the 'size' argument.
	void decryptCBC(char*const data, unsigned size)const;						// -Decrypts the message pointed by 'data'. The message must had been encrypted
																				//	using the CBC mode operation.
																				// -The size of the message is provided by the 'size' argument.

	void encrypt(char*const data, unsigned size)const;							// -Encrypts using operation mode stored in Key object
	void decrypt(char*const data, unsigned size)const;							// -Decrypts using operation mode stored in Key object

	void saveKey(const char*const fname) const {this->key.save(fname);}
	Key::OperationMode getOperationMode() const{ return this->key.getOperationMode(); }

	private:
	void XORblocks(char b1[AES_BLK_SZ], char b2[AES_BLK_SZ], char r[AES_BLK_SZ]) const;	// -Xor operation over 16 bytes array.
	void printWord(const char word[4]);											// -Prints an array of 4 bytes.
	void printState(const char state[AES_BLK_SZ]);								// -Prints an array of 16 bytes.
	void CopyWord(const char source[4], char destination[4]) const;				// -Coping an array of 4 bytes.
	void CopyBlock(const char source[AES_BLK_SZ], char destination[AES_BLK_SZ]) const;// -Coping an array of 16 bytes.
	void XORword(const char w1[4], const char w2[4], char resDest[4]) const;	// -XOR of arrays of 4 bytes.
	void RotWord(char word[4]) const;											// -Rotation of bytes to the left.
	void SubWord(char word[4]) const;											// -Apply SBox to each char of the word.
	void SubBytes(char state[AES_BLK_SZ]) const;								// -Applies a substitution table (S-box) to each char.
	void ShiftRows(char state[AES_BLK_SZ]) const;								// -Shift rows of the state array by different offset.
	void MixColumns(char state[AES_BLK_SZ]) const;								// -Mixes the data within each column of the state array.
	void AddRoundKey(char state[AES_BLK_SZ], int round) const;					// -Combines a round key with the state.
	void InvSubBytes(char state[AES_BLK_SZ]) const;								// -Applies the inverse substitution table (InvSBox) to each char.
	void InvShiftRows(char state[AES_BLK_SZ]) const;							// -Inverse function of shift rows.
	void InvMixColumns(char state[AES_BLK_SZ]) const;							// -Inverse function of MixColumns.
	void encryptBlock(char block[AES_BLK_SZ]) const;							// -Encrypts an array of 16 bytes.
	void decryptBlock(char block[AES_BLK_SZ]) const;							// -Decrypts an array of 16 bytes.
};
};
#endif