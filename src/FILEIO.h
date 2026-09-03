// Shared public interface for the FILEIO owner.
#ifndef FILEIO_INC_H
#define FILEIO_INC_H

#ifndef FILEIO_OWNER_DECLARATIONS
/** Current state of the asynchronous file transfer. */
extern int Loading __attribute__((section(".data")));

/** Prevents file I/O while the queued spooler changes transfer ownership. */
extern int SpoolerBlocksFileIO __attribute__((section(".data")));
#endif

/** Load a complete file into a newly allocated heap buffer. */
char *FileIO_OpenLoad(char *pName);

/** Set the maximum number of CD sectors transferred per update. */
void FileIO_SetSectorsPerFrame(int sectors);

/** Initialize the CD subsystem and locate the packed asset archive. */
void FileIO_Init(void);

/** Open an asset and return the byte count required for its transfer. */
int FileIO_Open(const char *filename);

/** Begin loading the currently open asset into the supplied buffer. */
void FileIO_Load(void *where);

/** Report whether an asset exists in the packed archive. */
int FileIO_Exists(char *filename);

/** Advance the asynchronous file-transfer state machine once. */
void FileIO_Continue(void);

/** Block until the current asynchronous file transfer completes. */
void FileIO_Sync(void);

/** Return the current asset subdirectory prefix. */
char *FileIO_GetSubDir(void);

/** Replace the current asset subdirectory prefix. */
void FileIO_SetSubDir(char *subDir);

#endif // FILEIO_INC_H
