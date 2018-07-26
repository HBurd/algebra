#include "algebra.h"

ElementInfo elements[MAX_ELEMENTS];
int num_elements = 0;

OperationInfo operations[MAX_OPERATIONS];
int num_operations = 0;

GroupInfo all_groups[MAX_GROUPS];
int num_groups = 0;

ElementInfo* declare_element(const char* name, GroupInfo* group, bool constant)
{
    elements[num_elements].name = name;
    elements[num_elements].group = group;
    elements[num_elements].constant = constant;

    return &elements[num_elements++];
}

OperationInfo* declare_operation(const char* name, GroupInfo* group)
{
    operations[num_operations].name = name;
    operations[num_operations].group = group;
    return &operations[num_operations++];
}

GroupInfo* declare_group(const char* operation, const char* identity)
{
    all_groups[num_groups].operation = declare_operation(operation, &all_groups[num_groups]);
    all_groups[num_groups].identity = declare_element(identity, &all_groups[num_groups], true);
    
    return &all_groups[num_groups++];
}


ElementInfo* identify_element(SubStr element)
{
    for (int i = 0; i < num_elements; i++)
    {
        if (element == elements[i].name)
        {
            return &elements[i];
        }
    }
    return nullptr;
}
