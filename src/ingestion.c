#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "ingestion.h"

static int safe_copy_path(char *dst, const char *src, size_t max_len) {
    size_t len = strlen(src);

    if (len >= max_len) {
        len = max_len - 1;  // enforce null termination
    }

    memcpy(dst, src, len);
    dst[len] = '\0';

    return 0;
}

static int add_file(const char *path, const struct stat *st, K501_DocumentSet *out) {
    K501_Document *tmp = realloc(out->docs, sizeof(K501_Document) * (out->count + 1));
    if (!tmp) return -1;

    out->docs = tmp;

    safe_copy_path(out->docs[out->count].path, path, K501_MAX_PATH);
    out->docs[out->count].size = st->st_size;

    out->count++;
    return 0;
}

static int traverse_dir(
    const char *path,
    int depth,
    int max_depth,
    K501_DocumentSet *out
) {
    if (depth > max_depth) return 0;

    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, alphasort);
    if (n < 0) return -1;

    for (int i = 0; i < n; i++) {
        if (namelist[i]->d_name[0] == '.') {
            free(namelist[i]);
            continue;
        }

        char fullpath[K501_MAX_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, namelist[i]->d_name);

        struct stat st;
        if (stat(fullpath, &st) == 0) {

            if (S_ISREG(st.st_mode)) {
                if (add_file(fullpath, &st, out) != 0) {
                    free(namelist[i]);
                    free(namelist);
                    return -1;
                }
            }
            else if (S_ISDIR(st.st_mode)) {
                if (traverse_dir(fullpath, depth + 1, max_depth, out) != 0) {
                    free(namelist[i]);
                    free(namelist);
                    return -1;
                }
            }
        }

        free(namelist[i]);
    }

    free(namelist);
    return 0;
}

int k501_ingest_directory_recursive(
    const char *path,
    int max_depth,
    K501_DocumentSet *out
) {
    if (!path || !out) return -1;

    out->docs = NULL;
    out->count = 0;

    return traverse_dir(path, 0, max_depth, out);
}