
#define JEU_BUILD_DLL

#include <math.h>
#include "image.h"

// --- Fonctions de base ---
JEU_API double abs_val(double x) { return fabs(x); }
JEU_API double clamp(double x, double minv, double maxv) {
    return fmax(minv, fmin(maxv, x));
}

// --- Puissances et racines ---
JEU_API double pow_custom(double base, double exp) { return pow(base, exp); }
JEU_API double sqrt_custom(double x) { return sqrt(x); }
JEU_API double cbrt_custom(double x) { return cbrt(x); }

// --- Logarithmes et exponentielles ---
JEU_API double exp_custom(double x) { return exp(x); }
JEU_API double log_custom(double x) { return log(x); }
JEU_API double log10_custom(double x) { return log10(x); }
JEU_API double log2_custom(double x) { return log2(x); }

// --- Trigonométrie ---
JEU_API double sin_custom(double x) { return sin(x); }
JEU_API double cos_custom(double x) { return cos(x); }
JEU_API double tan_custom(double x) { return tan(x); }
JEU_API double asin_custom(double x) { return asin(x); }
JEU_API double acos_custom(double x) { return acos(x); }
JEU_API double atan_custom(double x) { return atan(x); }
JEU_API double atan2_custom(double y, double x) { return atan2(y, x); }

// --- Hyperboliques ---
JEU_API double sinh_custom(double x) { return sinh(x); }
JEU_API double cosh_custom(double x) { return cosh(x); }
JEU_API double tanh_custom(double x) { return tanh(x); }
JEU_API double asinh_custom(double x) { return asinh(x); }
JEU_API double acosh_custom(double x) { return acosh(x); }
JEU_API double atanh_custom(double x) { return atanh(x); }

// --- Arrondis ---
JEU_API double floor_custom(double x) { return floor(x); }
JEU_API double ceil_custom(double x) { return ceil(x); }
JEU_API double round_custom(double x) { return round(x); }
JEU_API double trunc_custom(double x) { return trunc(x); }

// --- Divers ---
JEU_API double fmod_custom(double x, double y) { return fmod(x, y); }
JEU_API double hypot_custom(double x, double y) { return hypot(x, y); }
