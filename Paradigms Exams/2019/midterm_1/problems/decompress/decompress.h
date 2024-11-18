// Decompresses given RNE compressed data.
// Takes ownership of memory block pointed by passed data pointer.
// Char array pointed by data ends with ‘\0’.
// After returning data must be pointing to a memory block which contains decompressed
// information ownership of which is passed back to the caller.
void Decompress(char** data);
