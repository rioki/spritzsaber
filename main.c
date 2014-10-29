/*
  Copyright © 2014 Sean Farrell <sean.farrell@rioki.org>
  This work is free. You can redistribute it and/or modify it under the
  terms of the Do What The Fuck You Want To Public License, Version 2,
  as published by Sam Hocevar. See the COPYING.txt file for more details.
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "spritz.h"

enum Mode
{
    ENCRYPT,
    DECRYPT
};
 
void print_usage()
{
    printf("Usage: \n"
           "  spritzsaber [mode] [key] [input file] [output file]\n"
           "\n"
           "Options: \n"
           "  mode: encrypt / decrypt\n"
           "  key: encryption key, may be up to 245 chars long\n"
           "  input file: the input to read\n"
           "  output file: the input to write");
}
 
int main(int argc, char* argv[])
{
    enum Mode    mode;     
    char*        key;
    size_t       keylen;
    spritz_byte  iv[10];
    spritz_state state;
    size_t       i;
    FILE* in  =  stdin;
    FILE* out =  stdout;
    int          c;
    
    if (argc < 3)
    {
        print_usage();
    }
    
    if (strcmp(argv[1], "encrypt") == 0)
    {
        mode = ENCRYPT;
    }
    else if (strcmp(argv[1], "decrypt") == 0)
    {
        mode = DECRYPT;
    }
    else
    {
        printf("Unknown mode.\n");
        print_usage();
    }
    
    key = argv[2];
    keylen = strlen(key);    

    if (argc >= 4)
    {
        in = fopen(argv[3], "rb");
        if (in == NULL)
        {
            printf("Could not open %s for reading.", argv[3]);
            return -1;
        }
    }
    
    if (argc >= 5)
    {
        out = fopen(argv[4], "wb");
        if (out == NULL)
        {
            printf("Could not open %s for writing.", argv[3]);
            return -1;
        }
    }
    
    if (mode == ENCRYPT)
    {
        srand(time(NULL));               
        for (i = 0; i < 10; i++)
        {
            iv[i] = (spritz_byte)rand();
        }
        fwrite(iv, 1, 10, out);
    }
    else
    {
        // first 10 byted in data are iv
        int r;
        r = fread(iv, 1, 10, in);
        if (r != 10)
        {
            printf("Could not read iv.");
            return -1;
        }
    } 
    
    spritz_init(&state);
    spritz_absorb(&state, (spritz_byte*)key, keylen);
    spritz_absorb_stop(&state);
    spritz_absorb(&state, iv, 10);
        
    c = fgetc(in);
    while (c != EOF)
    {
        spritz_byte r;
        
        if (mode == ENCRYPT)
        {
            r = ((spritz_byte)c) + spritz_drip(&state);
        }
        else
        {
            r = ((spritz_byte)c) - spritz_drip(&state);
        }
        
        fputc(r, out);
        
        c = fgetc(in);
    }  

    return 0;
}
