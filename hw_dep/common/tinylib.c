//
// Copyright (c) 2017, Kentaro Sekimoto
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "tinylib.h"

#ifndef RZA1H
void *memset(void *dst, int c, size_t len) __attribute__((weak));
void *memset(void *dst, int c, size_t len)
{
    char *p;
    for (p = (char *)dst; len > 0; len--)
        *(p++) = c;
    return (void *)dst;
}
#endif

void *memcpy(void *dst, const void *src, size_t len)
{
    char *d = (char *)dst;
    const char *s = (const char *)src;
    for (; len > 0; len--)
        *(d++) = *(s++);
    return (void *)dst;
}

void *memmove(void *dst, void const *src, size_t len)
{
    char *d = (char *)dst;
    char const *s = (char const *)src;
    if (s < d)
        for (s += len, d += len; len; --len)
            *--d = *--s;
    else if (s != d) {
        for (; len; --len)
            *d++ = *s++;
    }
    return (void *)d;
}

int memcmp(const void *p1, const void *p2, size_t len)
{
    unsigned char *a, *b;
    size_t i;

    a = (unsigned char *)p1;
    b = (unsigned char *)p2;
    for (i = 0; i < len; i++) {
        if (*a != *b) {
            return (*a < *b) ? -1 : 1;
        }
        a++;
        b++;
    }
    return (int)0;
}

int strlen(const char *s)
{
    int len;
    for (len = 0; *s; s++, len++)
        ;
    return len;
}

char *strchr(char *s, int c)
{
    char c1;
    if (c == 0) {
        while (*s++)
            ;
        return s - 1;
    }
    while (c1 = *s++)
        if (c1 == c)
            return s - 1;
    return 0;
}

char *strcpy(char *dst, const char *src)
{
    char *d = dst;
    for (;; dst++, src++) {
        *dst = *src;
        if (!*src)
            break;
    }
    return d;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);
    return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

int strncmp(const char *p1, const char *p2, int len)
{
    for (; len > 0; len--) {
        if (*(p1++) != *(p2++))
            return 1;
    }
    return 0;
}

char *strncpy(char *dst, const char *src, size_t len)
{
    char *q = (char *)dst;
    const char *p = (const char *)src;
    char ch;
    while (len--) {
        *q++ = ch = *p++;
        if (!ch)
            break;
    }
    return dst;
}

void *bsearch(void *key, void *base0, size_t nmemb, size_t size, int (*compar)())
{
    register char *base = base0;
    register int lim, cmp;
    register void *p;

    for (lim = nmemb; lim != 0; lim >>= 1) {
        p = base + (lim >> 1) * size;
        cmp = (*compar)(key, p);
        if (cmp == 0)
            return (p);
        if (cmp > 0) { /* key > p: move right */
            base = (char *)p + size;
            lim--;
        } /* else move left */
    }
    return (NULL);
}

int atoi(const char *s)
{
    int result = 0, sign = 1;
    if (*s == (char)(-1)) {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9')
        result = result * 10 + (*(s++) - '0');
    return result * sign;
}

int isdigit(register int c)
{
    return ((c >= '0') && (c <= '9'));
}
