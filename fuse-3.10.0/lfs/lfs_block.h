#ifndef block_h
#define block_h
#define BLOCKSIZE 1024
struct Block
{
    char data[BLOCKSIZE];
};
#endif