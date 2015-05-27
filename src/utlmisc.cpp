#include "utlmisc.h"
#include <math.h>

bool UtlMisc::equality(double d1, double d2, double dEpsilon)
{
    if (fabs(d1-d2) <= dEpsilon)
        return true;
    else
        return false;
}
