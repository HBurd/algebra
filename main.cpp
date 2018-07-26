#include <iostream>
#include <cassert>
#include <emscripten.h>
#include <string>
#include <sstream>
#include <regex>

#include "latex_lexer.h"
#include "algebra.h"
#include "expression.h"
#include "parser.h"

extern "C"
{
    extern void init_math();
    extern void begin_row();
    extern void end_row();
    
    extern void display_math(const char* latex);
}

void generate_latex(Expression* expression, std::stringstream& expr_string)
{
    if (expression == nullptr) return;

    switch (expression->type)
    {
    case ExpressionType::BINARY_OP:
        expr_string << "\\left( ";
        generate_latex(expression->binary_operation.x, expr_string);
        expr_string << expression->binary_operation.info->name;
        generate_latex(expression->binary_operation.y, expr_string);
        expr_string << " \\right)";
        break;

    case ExpressionType::ELEMENT:
        expr_string << expression->element.info->name;
        break;

    case ExpressionType::INVERSE:
        generate_latex(expression->inverse.operand, expr_string);
        expr_string << "^{-1}";
        break;
    case ExpressionType::EXPONENT:
        generate_latex(expression->exponent.lhs, expr_string);
        expr_string << "^";
        generate_latex(expression->exponent.rhs, expr_string);
        break;

    default:
        assert(false);
    }
}

bool expression_equality(Expression* e1, Expression* e2)
{
    if (e1->type != e2->type)
    {
        return false;
    }

    switch (e1->type)
    {
    case ExpressionType::BINARY_OP:
        return (e1->binary_operation.info == e2->binary_operation.info)
            && expression_equality(e1->binary_operation.x, e2->binary_operation.x)
            && expression_equality(e1->binary_operation.y, e2->binary_operation.y);

    case ExpressionType::ELEMENT:
        return e1->element.info == e2->element.info;

    case ExpressionType::INVERSE:
        return expression_equality(e1->inverse.operand, e2->inverse.operand);
    default:
        return false;
    }
}

bool is_inverse(OperationInfo* operation, Expression* e1, Expression* e2)
{
    if (e1->type == ExpressionType::INVERSE && e1->inverse.info == operation)
    {
        return expression_equality(e1->inverse.operand, e2);
    }
    
    if (e2->type == ExpressionType::INVERSE && e2->inverse.info == operation)
    {
        return expression_equality(e2->inverse.operand, e1);
    }

    return false;
}

void cancel_inverse(Expression* operation)
{
    if (operation->type != ExpressionType::BINARY_OP) return;

    if (is_inverse(operation->binary_operation.info,
                   operation->binary_operation.x,
                   operation->binary_operation.y))
    {
        ElementInfo* identity = operation->binary_operation.info->group->identity;
        operation->type = ExpressionType::ELEMENT;
        operation->element.info = identity;
    }
}

extern "C"
{
    // called by js when an equation is edited
    void equation_edit_callback(const char* latex)
    {
        std::cout << "latex input: " << latex << std::endl;
        Expression* expr = parse_latex(latex);
        if (expr)
        {
            std::stringstream latex_output;
            generate_latex(expr, latex_output);
            std::cout << latex_output.str() << std::endl;
        }
        else
        {
            std::cout << "invalid expression" << std::endl;
        }
    }

}

int main(int argc, char** argv)
{
    GroupInfo* group = declare_group(" \\cdot ", "1");

    ElementInfo* a = declare_element("a", group, false);
    ElementInfo* b = declare_element("b", group, false);

    Expression* a1 = new_element(a);
    Expression* a2 = new_element(a);
    Expression* inv = new_inverse(group->operation, a2);
    Expression* times = new_binary_operation(group->operation, a1, inv);

    init_math();

    //std::stringstream expr_string;

    //generate_latex(times, expr_string);
    //begin_row();
    //display_math(expr_string.str().c_str());
    //end_row();

    return 0;
}
