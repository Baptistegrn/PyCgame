
#define JEU_BUILD_DLL
#include "image.h"
#include <stdbool.h>

bool debug = false;  

JEU_API void mettre_debug(bool cond) {
    debug = cond;
}
