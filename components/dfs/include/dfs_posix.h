/*
 * File      : dfs_posix.h
 * This file is part of Device File System in RT-Thread RTOS
 * COPYRIGHT (C) 2004-2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-05-27     Yi.qiu       The first version.
 * 2010-07-18     Bernard      add stat and statfs structure definitions.
 * 2011-05-16     Yi.qiu       Change parameter name of rename, "new" is C++ key word.
 * 2017-12-27     Bernard      Add fcntl API.
 * 2018-02-07     Bernard      Change the 3rd parameter of open/fcntl/ioctl to '...'
 */

#ifndef __DFS_POSIX_H__
#define __DFS_POSIX_H__

#include <dfs_file.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DFS_USING_C_FILE_EXTEND
#undef FILE
#undef fopen
#undef fclose
#undef fgets
#undef fflush
#undef ftell
#undef fseek
#undef fwrite
#undef fread
#undef rewind
#undef fprintf
#undef putc
#undef fputc
#undef fputs

#define _FREAD      0x0001  /* read enabled */
#define _FWRITE     0x0002  /* write enabled */
#define _FAPPEND    0x0008  /* append (writes guaranteed at the end) */
#define _FMARK      0x0010  /* internal; mark during gc() */
#define _FDEFER     0x0020  /* internal; defer for next gc pass */
#define _FASYNC     0x0040  /* signal pgrp when data ready */
#define _FSHLOCK    0x0080  /* BSD flock() shared lock present */
#define _FEXLOCK    0x0100  /* BSD flock() exclusive lock present */
#define _FCREAT     0x0200  /* open with file create */
#define _FTRUNC     0x0400  /* open with truncation */
#define _FEXCL      0x0800  /* error on open if file exists */
#define _FNBIO      0x1000  /* non blocking I/O (sys5 style) */
#define _FSYNC      0x2000  /* do all writes synchronously */
#define _FNONBLOCK  0x4000  /* non blocking I/O (POSIX style) */
#define _FNDELAY    _FNONBLOCK  /* non blocking I/O (4.2 style) */
#define _FNOCTTY    0x8000  /* don't assign a ctty on this open */

#define FASYNC		_FSYNC
#define FNDELAY		_FNDELAY

#if 0
#define O_RDONLY    0       /* +1 == FREAD */
#define O_WRONLY    1       /* +1 == FWRITE */
#define O_RDWR      2       /* +1 == FREAD|FWRITE */
#define O_APPEND    _FAPPEND
#define O_CREAT     _FCREAT
#define O_TRUNC     _FTRUNC
#define O_EXCL      _FEXCL
#define O_SYNC      _FSYNC
#endif

/*
r              open text file for reading
w              truncate to zero length or create text file for writing
wx             create text file for writing
a              append; open or create text file for writing at end-of-file
rb             open binary file for reading
wb             truncate to zero length or create binary file for writing
wbx            create binary file for writing
ab             append; open or create binary file for writing at end-of-file
r+             open text file for update (reading and writing)
w+             truncate to zero length or create text file for update
w+x            create text file for update
a+             append; open or create text file for update, writing at end-of-file
r+b  or rb+    open binary file for update (reading and writing)
w+b  or wb+    truncate to zero length or create binary file for update
w+bx or wb+x   create binary file for update
a+b  or ab+    append; open or create binary file for update, writing at end-of-file
*/

#define C_FILE_O_TEXT_EXIST_FILE_FOR_RDONLY                 "r"
#define C_FILE_O_TEXT_TRUNCATE_ZERO_OR_CREATE_FOR_WRONLY    "w"
#define C_FILE_O_TEXT_CREATE_FOR_WRONLY                     "wx"
#define C_FILE_O_TEXT_APPEND_FOR_WRONLY                     "a"
#define C_FILE_O_BINARY_EXIST_FILE_FOR_RDONLY               "rb"
#define C_FILE_O_BINARY_TRUNCATE_ZERO_OR_CREATE_FOR_WRONLY  "wb"
#define C_FILE_O_BINARY_CREATE_FOR_WRONLY                   "wbx"
#define C_FILE_O_BINARY_APPEND_FOR_WRONLY                   "ab"
#define C_FILE_O_TEXT_EXIST_FILE_FOR_RDWR                   "r+"
#define C_FILE_O_TEXT_TRUNCATE_ZERO_OR_CREATE_FOR_RDWR      "w+"
#define C_FILE_O_TEXT_CREATE_FOR_RDWR                       "w+x"
#define C_FILE_O_TEXT_APPEND_FOR_RDWR                       "a+"
#define C_FILE_O_BINARY_EXIST_FILE_FOR_RDWR_1               "r+b"
#define C_FILE_O_BINARY_EXIST_FILE_FOR_RDWR_2               "rb+"
#define C_FILE_O_BINARY_TRUNCATE_ZERO_OR_CREATE_FOR_RDWR_1  "w+b"
#define C_FILE_O_BINARY_TRUNCATE_ZERO_OR_CREATE_FOR_RDWR_2  "wb+"
#define C_FILE_O_BINARY_CREATE_FOR_RDWR_1                   "w+bx"
#define C_FILE_O_BINARY_CREATE_FOR_RDWR_2                   "wb+x"
#define C_FILE_O_BINARY_APPEND_FOR_RDWR_1                   "a+b"
#define C_FILE_O_BINARY_APPEND_FOR_RDWR_2                   "ab+"

#define FPRINTF_MAX_BUFFER_SIZE         128
#endif

typedef struct
{
    int fd;     /* directory file */
    char buf[512];
    int num;
    int cur;
} DIR;

/* directory api*/
int mkdir(const char *path, mode_t mode);
DIR *opendir(const char *name);
struct dirent *readdir(DIR *d);
long telldir(DIR *d);
void seekdir(DIR *d, off_t offset);
void rewinddir(DIR *d);
int closedir(DIR* d);

/* file api*/
int open(const char *file, int flags, ...);
int close(int d);
#ifdef RT_USING_NEWLIB
_READ_WRITE_RETURN_TYPE _EXFUN(read, (int __fd, void *__buf, size_t __nbyte));
_READ_WRITE_RETURN_TYPE _EXFUN(write, (int __fd, const void *__buf, size_t __nbyte));
#else
int read(int fd, void *buf, size_t len);
int write(int fd, const void *buf, size_t len);
#endif
off_t lseek(int fd, off_t offset, int whence);
int rename(const char *from, const char *to);
int unlink(const char *pathname);
int stat(const char *file, struct stat *buf);
int fstat(int fildes, struct stat *buf);
int fsync(int fildes);
int fcntl(int fildes, int cmd, ...);
int ioctl(int fildes, int cmd, ...);

/* directory api*/
int rmdir(const char *path);
int chdir(const char *path);
char *getcwd(char *buf, size_t size);

/* file system api */
int statfs(const char *path, struct statfs *buf);

int access(const char *path, int amode);
int pipe(int fildes[2]);
int mkfifo(const char *path, mode_t mode);

#ifdef DFS_USING_C_FILE_EXTEND
FILE * fopen(const char * path, const char * mode);
char *fgets(char *buf, int bufsize, FILE *stream);
int fclose( FILE *fp );
int fflush(FILE *stream);
long ftell(FILE *stream);
int fseek(FILE *stream, long offset, int fromwhere);
size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
size_t fread ( void *buffer, size_t size, size_t count, FILE *stream);
void rewind(FILE *stream);

int fprintf(FILE* fp, char* fmt, ...);
int putc(int ch, FILE *stream);
int fputc(int ch, FILE *stream);
int fputs(const char *s, FILE *stream);
#endif

#ifdef __cplusplus
}
#endif

#endif
