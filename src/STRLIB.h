#ifndef STRLIB_H
#define STRLIB_H

/** Finds the first occurrence of a substring, or null when it is absent. */
char *strstr(char *pHay, char *pNeedle);

/** Converts an ASCII string to lower case in place. */
void LowerCase(char *p);

/** Reports whether the current character is a space or horizontal tab. */
int WhiteSpace(char *p);

#endif // STRLIB_H
