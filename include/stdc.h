#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <common.h>

/*
 * This file contains declarations for subset of C standard library functions.
 * Implementation is imported from various places (mainly smallclib and OpenBSD)
 * and exposed for use within the kernel.
 *
 * There're extra utilities here as well.
 */

/* ctype.h function prototypes */

/* stdio.h function prototypes */

/* The snprintf has a custom implementation which cannot format
   floating-point numbers, see snprintf.c */
int snprintf(char *str, size_t size, const char *fmt, ...)
  __attribute__((format(printf, 3, 4)));

/* stdlib.h function prototypes */
void qsort(void *base, size_t num, size_t width,
           int (*comp)(const void *, const void *));

/* strings.h function prototypes */

void bzero(void *b, size_t length);
void *memchr(const void *s, int c, size_t n);
void *memcpy(void *s1, const void *s2, size_t n);
void *memset(void *dst, int c, size_t n);
int strcmp(const char *s1, const char *s2);
size_t strcspn(const char *s1, const char *s2);
size_t strlcat(char *dst, const char *src, size_t dsize);
size_t strlcpy(char *dst, const char *src, size_t dsize);
size_t strlen(const char *str);
int strncmp(const char *s1, const char *s2, size_t n);
char *strsep(char **stringp, const char *delim);
size_t strspn(const char *s1, const char *s2);

/* Write a formatted string to UART.
 * Equivalent to standard printf. */
int kprintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

/* Write a character string and a trailing newline to UART.
 * Equivalent to standard puts */
int kputs(const char *s);

/* Write a character to UART.
 * Equivalent to standard putchar */
int kputchar(int c);

#endif /* __STDLIB_H__ */
