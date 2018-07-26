#include "expression.h"

Expression* new_binary_operation(OperationInfo* info, Expression* x, Expression* y)
{
    Expression* expr = new Expression;
    expr->type = ExpressionType::BINARY_OP;
    expr->binary_operation.info = info;
    expr->binary_operation.x = x;
    expr->binary_operation.y = y;
    return expr;
}

Expression* new_element(ElementInfo* info)
{
    Expression* expr = new Expression;
    expr->type = ExpressionType::ELEMENT;
    expr->element.info = info;
    return expr;
}

Expression* new_inverse(OperationInfo* info, Expression* operand)
{
    Expression* expr = new Expression;
    expr->type = ExpressionType::INVERSE;
    expr->inverse.operand = operand;
    expr->inverse.info = info;
    return expr;
}

Expression* new_exponent(OperationInfo* info, Expression* lhs, Expression* rhs)
{
    Expression* expr = new Expression;
    expr->type = ExpressionType::EXPONENT;
    expr->exponent.info = info;
    expr->exponent.lhs = lhs;
    expr->exponent.rhs = rhs;
    return expr;
}
