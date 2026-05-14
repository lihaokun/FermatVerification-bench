/*
 * string_utils.c — 10-function string utility library
 * Multi-level call graph for verify-program testing.
 *
 * Call graph (4 levels):
 *   Level 0 (leaf): my_strlen, my_memset, char_is_alpha, char_to_lower
 *   Level 1: my_strcmp, my_memcpy
 *   Level 2: my_strcpy, str_to_lower
 *   Level 3: str_eq_nocase, str_copy_lower
 */

#include <stddef.h>

typedef unsigned char u8;

/* === Level 0: Leaf functions === */

/* Compute length of null-terminated string */
int my_strlen(const char *s)
{
    int len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

/* Fill n bytes of memory with value c */
void my_memset(u8 *dst, u8 c, int n)
{
    for (int i = 0; i < n; i++)
        dst[i] = c;
}

/* Check if character is alphabetic (A-Z or a-z) */
int char_is_alpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/* Convert uppercase to lowercase, leave others unchanged */
char char_to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}

/* === Level 1: Use Level 0 === */

/* Compare two strings lexicographically.
 * Returns 0 if equal, <0 if s1<s2, >0 if s1>s2 */
int my_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s1[i] == s2[i])
        i++;
    return (unsigned char)s1[i] - (unsigned char)s2[i];
}

/* Copy n bytes from src to dst (non-overlapping) */
void my_memcpy(u8 *dst, const u8 *src, int n)
{
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/* === Level 2: Use Level 0 + Level 1 === */

/* Copy string src to dst (including null terminator).
 * dst must have enough space. */
void my_strcpy(char *dst, const char *src)
{
    int len = my_strlen(src);
    my_memcpy((u8 *)dst, (const u8 *)src, len + 1);
}

/* Convert string to lowercase in-place */
void str_to_lower(char *s)
{
    int len = my_strlen(s);
    for (int i = 0; i < len; i++)
        s[i] = char_to_lower(s[i]);
}

/* === Level 3: Use Level 2 === */

/* Case-insensitive string equality check.
 * Modifies neither string (uses local buffers).
 * Limited to strings of length <= 255. Returns 1 if equal, 0 otherwise. */
int str_eq_nocase(const char *s1, const char *s2)
{
    int len1 = my_strlen(s1);
    int len2 = my_strlen(s2);
    if (len1 != len2)
        return 0;
    /* Compare character by character with lowering */
    for (int i = 0; i < len1; i++) {
        if (char_to_lower(s1[i]) != char_to_lower(s2[i]))
            return 0;
    }
    return 1;
}

/* Copy src to dst, converting to lowercase */
void str_copy_lower(char *dst, const char *src)
{
    my_strcpy(dst, src);
    str_to_lower(dst);
}
