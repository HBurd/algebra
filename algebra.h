#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "util.h"

const int MAX_ELEMENTS = 1024;
const int MAX_GROUPS = 1024;
const int MAX_OPERATIONS = 1024;

struct GroupInfo;

struct ElementInfo
{
    const char* name;
    GroupInfo* group;
    bool constant;
};

struct OperationInfo
{
    const char* name;
    GroupInfo* group;
};

struct GroupInfo
{
    OperationInfo* operation;
    ElementInfo* identity;
};

extern ElementInfo elements[MAX_ELEMENTS];
extern int num_elements;

extern OperationInfo operations[MAX_OPERATIONS];
extern int num_operations;

extern GroupInfo all_groups[MAX_GROUPS];
extern int num_groups;

ElementInfo* declare_element(const char* name, GroupInfo* group, bool constant);
OperationInfo* declare_operation(const char* name, GroupInfo* group);
GroupInfo* declare_group(const char* operation, const char* identity);

ElementInfo* identify_element(SubStr element);

#endif
