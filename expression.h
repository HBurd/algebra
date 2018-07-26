#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "algebra.h"

enum class ExpressionType
{
    BINARY_OP,
    ELEMENT,
    INVERSE,
    EXPONENT
};

enum DisplayInfoFlags
{
    DISPLAY_INFO_BRACKETED = 1
};

struct DisplayInfo
{
    int flags;
};

struct Expression
{
    ExpressionType type;
    DisplayInfo display_info;
    union
    {
        struct
        {
            OperationInfo* info;
            Expression* x;
            Expression* y;
        } binary_operation;

        struct
        {
            ElementInfo* info;
        } element;

        struct
        {
            OperationInfo* info;  //operation of inverse
            Expression* operand;
        } inverse;

        struct
        {
            OperationInfo* info;
            Expression* lhs;
            Expression* rhs;
        } exponent;
    };
};

Expression* new_binary_operation(OperationInfo* info, Expression* x, Expression* y);
Expression* new_element(ElementInfo* info);
Expression* new_inverse(OperationInfo* info, Expression* operand);
Expression* new_exponent(OperationInfo* info, Expression* lhs, Expression* rhs);

#endif
