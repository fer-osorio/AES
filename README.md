#  AES

AES (Advance Encryption Standard) implementation in C++, applied to the encryption of BMP images and text files.

Mail me at aosorios1502@alumno.ipn.mx and alexis.fernando.osorio.sarabio@gmail.com for questions and comments.

#  Overview

Implementation of symmetric block cipher AES (published by [NIST](https://www.nist.gov/)) in a name space called AES. Specification of this standard can be found [here](https://www.nist.gov/publications/advanced-encryption-standard-aes-0). Then, AES class, together with other structures, is used to encrypt Text files and BMP images.

At the moment of writing this README, the supported operation modes are:

- ECB [Electronic Code Book]
- CBC [Cipher Block Chaining]
- PVS [Pi and Variable Sbox] (Experimental, not specified in the standard)

And all key length specified in the standard (that is: 128, 192 and 256 bits) are supported. Also, the supported files are

- Text files (.txt)
- Bitmap images (.bmp)

**Important notes**:
* AES name space does not implement any method to obtain secure cryptographic keys.
* Padding problem for the ECB and CBC mode is solved with a method not specified in the NIST standard.

#   Usage

##  Creating a Cipher object
Before encrypting anything, we need to establish a cryptographic key length, a cryptographic key and a operation mode. All this objects are specified inside a structure called **Key** inside the same name space **AES**. A **Key** object can be created by

1. Using ``Key(const char* const _key, Length, OperationMode, const char* const _IV = NULL);``. This establish all attributes manually. The ``_IV`` argument stands for "Initial Vector"; is intended for CBC mode.
2. Using ``Key(const char*const fname)``. This builds the key from a binary file.

This structure is necessary to create a Cipher object because the unique constructor it has (apart from default constructor and copy constructor) is ``Cipher(const Key&)``. The intention of this is to have a well constructed cryptographic key before use it to encrypt.

***In summary, to create a Cipher object:***
1. Create a **Key** object, either manually or from a file.
2. Use the constructor ``Cipher(const Key&)``.

**Important note**: The default constructor for Cipher, ``Chipher()``, sets each byte of key and key expansion as zero, so is mandatory to not use this constructor for an actual encryption application.

## Encryption and decryption

Once we have a Cipher object, for encryption it is only necessary to invoke the member function 

```
void encrypt(char*const data, unsigned size)const
```

Same for encryption

```
void decrypt(char*const data, unsigned size)const
```

Encryption and decryption process will succeed if and only if the Cipher objects used in each end have the same key.

***Important note***: Each of the two functions above will act on the bytes pointed by **data** without creating a copy of the original content.

### Encrypting and decrypting files.

To this end, ``encrypt`` and ``decrypt`` functions are overloaded so they can accept a file structure and a Cipher object as arguments. In higher detail,

```
friend void encrypt(Bitmap& bmp, AES::Cipher& e)
```

encrypts ``bmp`` Bitmap file using the Cipher object ``e``. Notice this last function is a friend of class ``Bitmap``, this has two intentions: First, to be capable to encrypt the bmp file data while maintaining its attributes private, and second, to have the possibility of encrypt several files with one single Cipher file.

Similarly, function
```
friend void decrypt(Bitmap& bmp, AES::Cipher& e)
```
decrypts ``bmp`` Bitmap file using Cipher object ``e``.

The same is true for the rest of the files supported.

```
friend void encrypt(TXT& txt, AES::Cipher& e)
```

```
friend void decrypt(TXT& txt, AES::Cipher& e)
```

#  Executable Files

There are two executables in Directory [Apps/Executables](Apps/Executables). Instructions for the building of this file can be found [here](Apps/Executables/README.md)
