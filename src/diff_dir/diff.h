#ifndef DIFF_DIR_H_INCLUDED
#define DIFF_DIR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/* Prototypes */
void sec_closedir(DIR *d);
DIR *sec_opendir(const char* path);
const char* get_type_char(struct stat* s);

int dir_search(const char* to_search, DIR* d);

int diff_dir(const char* dir_name1, const char* dir_name2);
int diff_dir_make(const char* d1_name, DIR* d1, const char* d2_name, DIR* d2);

#endif // DIFF_DIR_H_INCLUDED
