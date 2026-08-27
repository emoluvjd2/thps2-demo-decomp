#ifndef DEBUG_INC_H
#define DEBUG_INC_H

/**
 * Receive a source-located debug message.
 *
 * @param p Debug message text.
 * @param line Source line that produced the message.
 * @param file Source file that produced the message.
 */
void Debug_String(char *p, int line, char *file);

#endif // DEBUG_INC_H
