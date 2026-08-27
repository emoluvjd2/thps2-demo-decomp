// String helpers used by game-script parsing.

#include "STRLIB.h"

/**
 * Finds the first occurrence of a substring.
 *
 * @param pHay String to search.
 * @param pNeedle Substring to locate.
 * @return A pointer to the first match, or null when no match exists.
 */
char *strstr(char *pHay, char *pNeedle) {
    char First = *pNeedle;
    pNeedle++;
    if (First != '\0') goto scan;
    return 0;
scan:
    {
        char hayCharacter = *pHay;
        pHay++;
        if (hayCharacter == First) goto found;
        if (hayCharacter != '\0') goto scan;
    }
    return 0;
found:
    {
        // MAIN.SYM places pH in $a3. Measured ordinary-local variants make
        // GCC swap it with the character temporary (six real mismatches).
        register char *pH asm("$7") = pHay;
        char *pN = pNeedle;
inner:
        {
            char needleCharacter = *pN;
            pN++;
            if (needleCharacter == '\0') return pHay - 1;
            char hayCharacter = *pH;
            pH++;
            if (needleCharacter == hayCharacter) goto inner;
            goto scan;
        }
    }
}

/**
 * Converts an ASCII string to lower case in place.
 *
 * @param p Null-terminated string to modify.
 */
void LowerCase(char *p) {
    if (*p == '\0') return;
    do {
        char letter = *p;
        if (static_cast<unsigned int>(letter - 'A') < 26) {
            letter += 'a' - 'A';
        }
        *p = letter;
        p++;
    } while (*p != '\0');
}

/**
 * Reports whether the current character is horizontal whitespace.
 *
 * @param p Character position to inspect.
 * @return One for a space or tab; otherwise zero.
 */
int WhiteSpace(char *p) {
    if (*p == ' ' || *p == '\t') {
        return 1;
    }
    return 0;
}
