// KeyExpansion routine headers.

// -Notice that the value of the left most byte 
//  in polynomial form is Rcon[i] == 2^i.
byte Rcon[] = { 0x01000000,
						 		0x02000000,
							 	0x04000000,
								 0x08000000,
 								0x10000000,
	 							0x20000000,
		 						0x40000000,
			 					0x80000000,
				 				0x1B000000,
					 			0x36000000 }

// -Standard rotation of bits to the left.
ui32 RotLeft(ui32 word, byte n);

// -Rotation of bytes to the left.
ui32 RotWord(ui32 word, byte n);

// -SubWord transformation.
ui32 SubWord(ui32 word, byte n);
