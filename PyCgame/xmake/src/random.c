
#define JEU_BUILD_DLL
#include "image.h"
#include <stdbool.h>


JEU_API int random_jeu(int min, int max) {
    return min + rand() % (max - min + 1);
}
