#include "util.h"
#include <cstring>

bool operator==(const SubStr& lhs, const SubStr& rhs)
{
    if (lhs.end - lhs.start != rhs.end - rhs.start) return false;

    for (int i = 0; i < lhs.end - lhs.start; i++)
    {
        if (lhs.str[i + lhs.start] != rhs.str[i + rhs.start]) return false;
    }

    return true;
}


bool operator==(const SubStr& lhs, const char* rhs)
{
    SubStr rhs_substr;
    rhs_substr.str = rhs;
    rhs_substr.start = 0;
    rhs_substr.end = strlen(rhs);

    return lhs == rhs_substr;
}

bool operator==(const char* lhs, const SubStr& rhs)
{
    return rhs == lhs;
}
