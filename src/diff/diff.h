/* == Inlcudes == */
#include "../constant.h"
#include "../index/index.h"
#include "subsq.h"
#include "../print/print.h"

#ifndef _H_DIFF_S
#define _H_DIFF_S
/* == Structures/Enum == */
typedef enum _File_type {
    T_DIR, T_FILE, T_STDIN, T_NONE
}File_type;

typedef struct _File{

	FILE *f;

	Index *i;

	char *path;
	char *label;

	struct stat st;
	File_type type;

}File;

#endif

#ifndef _H_DIFF
#define _H_DIFF

/* == Prototypes == */
FILE* sec_fopen(const char* path, const char* mode);
void sec_fclose(FILE *f);
int diff_file(File files[]);

#endif
