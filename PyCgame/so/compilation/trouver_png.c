#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_MAX MAX_PATH
#else
    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <limits.h>
    #include <errno.h>
#endif

int ends_with_png(const char *name) {
    if (!name) return 0;
    size_t len = strlen(name);
    if (len < 4) return 0;
    const char *ext = name + len - 4;
    return (tolower(ext[0]) == '.' &&
            tolower(ext[1]) == 'p' &&
            tolower(ext[2]) == 'n' &&
            tolower(ext[3]) == 'g');
}

#ifdef _WIN32
int collect_pngs(const char *dir, char ***out_list, int *out_count) {
    if (!dir || !out_list || !out_count) {
        if (debug) fprintf(stderr, "DEBUG: collect_pngs argument invalide\n");
        return -1;
    }

    char searchPath[PATH_MAX];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", dir);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(searchPath, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        if (debug) fprintf(stderr, "DEBUG: FindFirstFile failed dir=%s\n", dir);
        return -1;
    }

    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
            continue;

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s\\%s", dir, fd.cFileName);

        normaliser_chemin(fullpath);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            collect_pngs(fullpath, out_list, out_count);
        } else if (ends_with_png(fd.cFileName)) {
            char **tmp = realloc(*out_list, sizeof(char*) * (*out_count + 1));
            if (!tmp) {
                FindClose(hFind);
                if (debug) fprintf(stderr, "DEBUG: realloc collect_pngs failed\n");
                return -1;
            }
            *out_list = tmp;
            (*out_list)[*out_count] = _strdup(fullpath);
            if (!(*out_list)[*out_count]) {
                FindClose(hFind);
                if (debug) fprintf(stderr,"DEBUG: strdup failed\n");
                return -1;
            }
            if (debug) fprintf(stderr,"DEBUG: png trouve %s\n", fullpath);
            (*out_count)++;
        }
    } while (FindNextFile(hFind, &fd));

    FindClose(hFind);
    return 0;
}
#else
int collect_pngs(const char *dir, char ***out_list, int *out_count) {
    if (!dir || !out_list || !out_count) {
        if (debug) fprintf(stderr, "DEBUG: collect_pngs argument invalide\n");
        return -1;
    }

    DIR *dp = opendir(dir);
    if (!dp) {
        if (debug) fprintf(stderr, "DEBUG: opendir failed dir=%s: %s\n", dir, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            if (debug) fprintf(stderr, "DEBUG: stat failed %s: %s\n", fullpath, strerror(errno));
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            collect_pngs(fullpath, out_list, out_count);
        } else if (ends_with_png(entry->d_name)) {
            char **tmp = realloc(*out_list, sizeof(char *) * (*out_count + 1));
            if (!tmp) {
                closedir(dp);
                if (debug) fprintf(stderr, "DEBUG: realloc collect_pngs failed\n");
                return -1;
            }
            *out_list = tmp;
            (*out_list)[*out_count] = strdup(fullpath);
            if (!(*out_list)[*out_count]) {
                closedir(dp);
                if (debug) fprintf(stderr, "DEBUG: strdup failed\n");
                return -1;
            }
            if (debug) fprintf(stderr, "DEBUG: png trouve %s\n", fullpath);
            (*out_count)++;
        }
    }

    closedir(dp);
    return 0;
}
#endif
