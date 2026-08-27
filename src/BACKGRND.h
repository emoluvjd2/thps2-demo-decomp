// Public declarations for BACKGRND.cpp.
#ifndef BACKGRND_INC_H
#define BACKGRND_INC_H

#include "globals.h"

struct CBody;
struct CBackground;

/** Enables the background object with the requested index. */
void Backgrnd_On(unsigned short index);

/** Disables the background object with the requested index. */
void Backgrnd_Off(unsigned short index);

/** Head of the active background-object body list. */
#define BODYLISTS_DECLARE_BACKGROUND_LIST
#define BODYLISTS_GP_RELATIVE_DECLARATIONS
#include "BODYLISTS.h"

/** Name of the object file currently being interpreted. */
#include "CURRENTOBJFILE.h"

#endif // BACKGRND_INC_H
