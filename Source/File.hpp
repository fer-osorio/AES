// -Set of structures representing files. The intention is to handle files for its encryption
#include<cstdint>
#include"AES.hpp"
#include <iomanip>

#ifndef _INCLUDED_FILE_
#define _INCLUDED_FILE_
#define NAME_MAX_LEN 4096
#define RGB_COMPONENTS_AMOUNT	3
#define DIRECTIONS_AMOUNT	3

namespace File {

/*
Valid file name or path grammar

'l' will denote letters in English alphabet, either lower case or upper case.
	For convenience, we will admit '_' and '-' as letters
'd' for digits 0, 1,..., 9
Sld	string of letters and digits that always starts with a letter
FN  File Name
PT  Path
Sled string of letters, digits and spaces. It never starts or ends with a space
FNWE File Name With Spaces

Sld	->	l·Sld	| l·d·Sld	|	l	|	l·d		// -Concatenation of letters and digits that always start with a letter
Sled->	l·SPACES·Sled	| l·d·SPACES·Sled	|	l	|	l·d	// -Concatenation of letters and digits that always start with a letter

FN	->	Sld·FN	|	l						// -File Name can not start with a digit; a single letter can be a File Name
FN	->	.Sld·FN |	FN.Sld·FN	|	.Sld			// -Can not finish with a point nor have two consecutive points

FN  ->  "FNWE"                                                                  // -If double quotes are presented at the beginning of the string, the grammar
FNWE->	Sled·FN	|	l							//  accepts spaces in the middle of the string until the next double quote is found
FNWE->	.Sled·FN|	FN.Sled·FN	|	.Sled

FN  ->  FN/·Sld·FN   |   ../FN	|	/·Sld·FN				// -Considering file paths (Absolute Paths and Relative Paths) as file names

Note: SPACES can be represented by single spaces, or a concatenation of spaces
*/

struct StringFileNameAnalize {
	public:	enum Extension { bmp, txt, NoExtension, Unrecognized };
	private:static const Extension  SupportedExtension[2];
	private:static const size_t	SupportedExtensionAmount;
	private:static const char* 	SupportedExtensionString[2];
	private:static const size_t 	extensionStringAmount;
	private:
	enum CharType {zero, letter, digit, dot, underscore, hyphen, slash, space, singleQuote ,doubleQuote, notAllowed};

	const char* str = NULL;
	size_t      size = 0;
	unsigned    currentIndex = 0;

	static	CharType characterType(const char c);				// -True for the character that may appear in the file name, false in other case
	void 	cerrSyntaxErrMsg(const char[]);

	bool Sld();								// -The returned bool flags the founding of zero byte or the characters '\'' or '"')
	bool FN ();

	StringFileNameAnalize(const char str_[]);

	public:
	static Extension getExtension(const char[]);				// -Compares its input with the strings in supportedExtension array
	static bool isValidFileName(const char str[]);
};

class TXT {									// -Handling .txt files
	char* name 	= NULL;
	char* content	= NULL;							// -Text file content.
	unsigned size	= 0;

	public:
	TXT() : name() {}							// -Just for type declaration.
	TXT(const char* fname);							// -Building from file.
	TXT(const TXT&);
	~TXT() {
		if(this->name != NULL) delete [] this->name;
		this->name = NULL;
		if(this->content != NULL) delete[] this->content;
		this->content = NULL;
		this->size = 0;
	}

	TXT& operator = (const TXT&);
	void save(const char* fname = NULL) const;
	void printName() const{ if(this->name != NULL) std::cout << this->name; }

	friend void encrypt(TXT& txt, AES::Cipher& e) {				// -Encrypts using the operation mode defined in Key object
		e.encrypt(txt.content, txt.size);
		txt.save();							// -The reason of the existence of these friend functions is to be capable of
	}									//  encrypt and decrypt many files with the same Cipher object while maintaining
										//  attributes of txt object private
	friend void decrypt(TXT& txt, AES::Cipher& e) {				// -Decrypts using the operation mode defined in Key object
		e.decrypt(txt.content, txt.size);
		txt.save();
	}
};

class Bitmap;									// -The intention is to use the name Bitmap in the next function
std::ostream& operator << (std::ostream& st, const Bitmap& bmp);		// -What we want is to make this function visible inside the name space scope
struct BitmapStatistics;
class Bitmap {									// -Handling bitmap format images.
	public: enum ColorID{ Red, Green, Blue};
	public: enum Direction{ horizontal, vertical, diagonal };
	public: static const char*const RGBlabels[RGB_COMPONENTS_AMOUNT];
	public: static const char*const DirectionLabels[DIRECTIONS_AMOUNT];
	private:
	struct RGB {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};
	struct FileHeader {
		char     bm[2];							// [B, M] for bmp files
		unsigned size;  						// File size
		uint16_t reserved1;						// Dependent on the originator application
		uint16_t reserved2;						// Dependent on the originator application
		uint32_t offset;						// Starting address of the image data
	} fh = {0,0,0,0,0,0};

	struct ImageHeader {
		uint32_t size;							// Size of this header
		int 	 Width;							// Image width in pixels
		int	 Height;						// Image height in pixels
		uint16_t Planes;						// Number of color planes (must be 1)
		uint16_t BitsPerPixel;						// Number of bits per pixel, in this case 24
		uint32_t Compression;						// Compression method
		uint32_t SizeOfBitmap;						// Size of raw bitmap data
		int	 HorzResolution; 					// Horizontal pixel per meter
		int	 VertResolution; 					// Vertical pixel per meter
		uint32_t ColorsUsed;						// Colors in the color palette, 0 to default
		uint32_t ColorsImportant;					// Zero when every color is important
	} ih = {0,0,0,0,0,0,0,0,0,0,0};

	size_t pixelAmount = 0;
	size_t bytesPerPixel = 3;

	char* data = NULL;
	RGB** img  = NULL;
	char* name = NULL;

	uint8_t getPixelColor(int i, int j, ColorID CId) const;


	public:
	Bitmap() {} 								// -Just for type declaration
	Bitmap(const char* fname);
	Bitmap(const Bitmap& bmp);
	~Bitmap();

	void save(const char* fname) const;					// -Saves in memory
	Bitmap& operator = (const Bitmap& bmp);
	friend std::ostream& operator << (std::ostream& st, const Bitmap& bmp);

	bool operator == (const Bitmap& bmp) const;
	bool operator != (const Bitmap& bmp) const;

	void writeBmpName(char destination[]) const;

	size_t PixelAmount() const{ return this->pixelAmount; }
	size_t dataSize() const{ return this->ih.SizeOfBitmap; }

	friend void encrypt(Bitmap& bmp, AES::Cipher& e, bool save = true, const char* newName = NULL) {// -Encrypts using the operation mode defined in Key object
		e.encrypt(bmp.data, bmp.ih.SizeOfBitmap);			// -The reason of the existence of these friend functions is to be capable of
    		if(save){							//  encrypt and decrypt many files with the same Cipher object while maintaining
    			if(newName != NULL) bmp.save(newName); 			//  attributes of bmp object private
    			else bmp.save(bmp.name);
    		}
	}
	friend void decrypt(Bitmap& bmp, AES::Cipher& e, bool save = true, const char* newName = NULL) {	// -Decrypts using the operation mode defined in Key object
		e.decrypt(bmp.data, bmp.ih.SizeOfBitmap);
    		if(save) {
    			if(newName != NULL) bmp.save(newName);
    			else bmp.save(bmp.name);
    		}
	}

	friend BitmapStatistics;
};

std::ostream& operator << (std::ostream& os, const BitmapStatistics& bmSt);
struct BitmapStatistics{
	private:
	const  Bitmap* 	  pbmp		= NULL;
	double Average    [RGB_COMPONENTS_AMOUNT]  = {-1.0};
	double Covariance [RGB_COMPONENTS_AMOUNT][DIRECTIONS_AMOUNT]  = {{ 0.0, 0.0, 0.0},{ 0.0, 0.0, 0.0},{ 0.0, 0.0, 0.0}};
	double Variance   [RGB_COMPONENTS_AMOUNT][DIRECTIONS_AMOUNT]  = {{-1.0,-1.0,-1.0},{-1.0,-1.0,-1.0},{-1.0,-1.0,-1.0}};
	double Correlation[RGB_COMPONENTS_AMOUNT][DIRECTIONS_AMOUNT]  = {{10.0,10.0,10.0},{10.0,10.0,10.0},{10.0,10.0,10.0}};

	uint32_t histogram[RGB_COMPONENTS_AMOUNT][256] = {{0},{0},{0}};
	bool	 histogramStablished = false;

	double Entropy    [RGB_COMPONENTS_AMOUNT]  = { 0.0};
	double XiSquare   [RGB_COMPONENTS_AMOUNT]  = { 0.0};

	double average(    const Bitmap::ColorID) const;			// -Average value of color in a range of pixels. Horizontal calculation
	double covariance( const Bitmap::ColorID, Bitmap::Direction dr, size_t offset) const;
	double variance(   const Bitmap::ColorID, Bitmap::Direction dr) const;
	double correlation(const Bitmap::ColorID, Bitmap::Direction dr, size_t offset) const;
	double entropy(const Bitmap::ColorID) const;
	double xiSquare(const Bitmap::ColorID)const;

	void sethistogram();

	public:
	BitmapStatistics() {}
	BitmapStatistics(const BitmapStatistics&);
	BitmapStatistics(const Bitmap* pbmp_);
	BitmapStatistics& operator = (const BitmapStatistics&);

	double retreaveCorrelation(const Bitmap::ColorID CID, Bitmap::Direction dr, double* xAxis_dest = NULL, double* yAxis_dest = NULL) const;
	double retreaveEntropy(const Bitmap::ColorID CID) const{ return this->Entropy[CID]; }
	double retreaveXiSquare(const Bitmap::ColorID CID) const{ return this->XiSquare[CID]; }
	size_t pixelAmount() const{ return this->pbmp->PixelAmount(); }
	void   writeHistogram(Bitmap::ColorID CID, double destination[]) const{ for(int i = 0; i < 256; i++) destination[i] = this->histogram[CID][i]; }

	friend std::ostream& operator << (std::ostream& os, const BitmapStatistics& bmSt);
	void writeBmpName(char destination[]) const{ this->pbmp->writeBmpName(destination); }
};
};
#endif