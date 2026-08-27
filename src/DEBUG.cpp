#include "DEBUG.h"

/**
 * Receives a source-located debug message.
 *
 * The prototype build intentionally discards the message.
 *
 * @param p Debug message text.
 * @param line Source line that produced the message.
 * @param file Source file that produced the message.
 */
void Debug_String(char *p, int line, char *file)
{
    (void)p;
    (void)line;
    (void)file;
}
