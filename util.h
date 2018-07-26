#ifndef UTIL_H
#define UTIL_H

struct SubStr
{
    const char* str;
    int start;
    int end;

};

bool operator==(const SubStr& lhs, const SubStr& rhs);
bool operator==(const SubStr& lhs, const char* rhs);
bool operator==(const char* lhs, const SubStr& rhs);

#endif
