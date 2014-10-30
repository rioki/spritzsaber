/*
  Copyright © 2014 Sean Farrell <sean.farrell@rioki.org>
  This work is free. You can redistribute it and/or modify it under the
  terms of the Do What The Fuck You Want To Public License, Version 2,
  as published by Sam Hocevar. See the COPYING.txt file for more details.
*/

#include "spritz.h"

spritz_byte low(spritz_byte b)
{
    return b & 0x0F;
}

spritz_byte high(spritz_byte b)
{
    return (b & 0xF0) >> 4;
}

void swap(spritz_byte* a, spritz_byte* b)
{
    spritz_byte t = *a;
    *a = *b;
    *b = t;
}

spritz_byte gcd(spritz_byte u, spritz_byte v)
{
    if (u == v)
    {
        return u;
    }
 
    if (u == 0)
    {
        return v;
    }
 
    if (v == 0)
    {
        return u;
    }
 
    if (~u & 1)
    {
        if (v & 1)
            return gcd(u >> 1, v);
        else 
            return gcd(u >> 1, v >> 1) << 1;
    }
 
    if (~v & 1)
    {
        return gcd(u, v >> 1);
    }
    
    if (u > v)
    {
        return gcd((u - v) >> 1, v);
    }
 
    return gcd((v - u) >> 1, u);
}

void update(spritz_state* state)
{
    state->i = state->i + state->w;
    state->j = state->k + state->S[(spritz_byte)(state->j + state->S[state->i])];
    state->k = state->i + state->k + state->S[state->j];
    swap(&state->S[state->i], &state->S[state->j]);
}

void whip(spritz_state* state, size_t r)
{
    size_t v;
    
    for (v = 0; v < r; v++)
    {
        update(state);
    }
    do 
    {
        state->w = state->w + 1;
    }
    while (gcd(state->w, (spritz_byte)SPRITZ_N) != (spritz_byte)1);
}

void crush(spritz_state* state)
{
    size_t v;
    
    for (v = 0; v < SPRITZ_N / 2; v++)    
    {
        if (state->S[v] > state->S[SPRITZ_N - 1 - v])
        {
            swap(&state->S[v], &state->S[SPRITZ_N - 1 - v]);
        }
    }
}

void shuffle(spritz_state* state)
{
    whip(state, SPRITZ_N * 2);
    crush(state);
    whip(state, SPRITZ_N * 2);
    crush(state);
    whip(state, SPRITZ_N * 2);
    state->a = 0;
}

void absorb_nibble(spritz_state* state, spritz_byte x)
{
    if (state->a == SPRITZ_N / 2)
    {
        shuffle(state);
    }
    swap(&state->S[state->a], &state->S[SPRITZ_N / 2 + x]);
    state->a = state->a + 1;
}

void absorb_byte(spritz_state* state, spritz_byte b)
{
    absorb_nibble(state, low(b));
    absorb_nibble(state, high(b));
}

void spritz_init(spritz_state* state)
{
    size_t v;
    
    state->i = 0;
    state->j = 0;
    state->k = 0;
    state->z = 0;
    state->a = 0;
    state->w = 1;
    
    for (v = 0; v < SPRITZ_N; v++)
    {
        state->S[v] = v;
    }
}

void spritz_absorb(spritz_state* state, spritz_byte* i, size_t ilen)
{
    size_t v;
    
    for (v = 0; v < ilen; v++)
    {
        absorb_byte(state, i[v]);
    }
}

void spritz_absorb_stop(spritz_state* state)
{
    if (state->a == SPRITZ_N / 2)
    {
        shuffle(state);
    }
    state->a = state->a + 1;
}

spritz_byte spritz_drip(spritz_state* state)
{
    if (state->a != 0)
    {
        shuffle(state);    
    }
    
    update(state);
    
    // output
    state->z = state->S[(spritz_byte)(state->j + state->S[(spritz_byte)(state->i + state->S[(spritz_byte)(state->z + state->k)])])];
    return state->z;    
}
