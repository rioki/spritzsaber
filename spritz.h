/*
  Copyright © 2014 Sean Farrell <sean.farrell@rioki.org>
  This work is free. You can redistribute it and/or modify it under the
  terms of the Do What The Fuck You Want To Public License, Version 2,
  as published by Sam Hocevar. See the COPYING.txt file for more details.
*/

#ifndef _SPRITZ_H_
#define _SPRITZ_H_

#include <stdlib.h>

#define SPRITZ_N 256

typedef unsigned char spritz_byte;

struct spritz_state_
{
    spritz_byte i;
    spritz_byte j;
    spritz_byte k;
    spritz_byte z;
    spritz_byte a;
    spritz_byte w;
    spritz_byte S[SPRITZ_N];
};
typedef struct spritz_state_ spritz_state;

void spritz_init(spritz_state* state);

void spritz_absorb(spritz_state* state, spritz_byte* i, size_t ilen);

void spritz_absorb_stop(spritz_state* state);

spritz_byte spritz_drip(spritz_state* state);

#endif
