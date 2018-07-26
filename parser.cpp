#include "parser.h"
#include "latex_lexer.h"
#include "algebra.h"
#include "util.h"
#include <iostream>

enum class TokenType
{
    COMMAND,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_CURLY,
    CLOSE_CURLY,
    EXPONENT,
    SUB,
    LETTER,
    MINUS,
    NUMBER,
    UNKNOWN
};

const char* get_token_type_string(TokenType type)
{
    switch (type)
    {
    case TokenType::COMMAND:
        return "COMMAND";
    case TokenType::OPEN_BRACKET:
        return "OPEN_BRACKET";
    case TokenType::CLOSE_BRACKET:
        return "CLOSE_BRACKET";
    case TokenType::OPEN_CURLY:
        return "OPEN_CURLY";
    case TokenType::CLOSE_CURLY:
        return "CLOSE_CURLY";
    case TokenType::EXPONENT:
        return "EXPONENT";
    case TokenType::SUB:
        return "SUB";
    case TokenType::LETTER:
        return "LETTER";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::NUMBER:
        return "NUMBER";
    default:
        return "UNKNOWN";
    };
}

TokenType classify_token(SubStr token)
{
    // for now we can completely classify tokens based on their first character
    char token_char = token.str[token.start];
    switch (token_char)
    {
    case '^':
        return TokenType::EXPONENT;
    case '_':
        return TokenType::SUB;
    case '{':
        return TokenType::OPEN_CURLY;
    case '}':
        return TokenType::CLOSE_CURLY;
    case '(':
        return TokenType::OPEN_BRACKET;
    case ')':
        return TokenType::CLOSE_BRACKET;
    case '-':
        return TokenType::MINUS;
    default:
        if ((token_char >= 'A' && token_char  <= 'Z')
            || (token_char  >= 'a' && token_char  <= 'z'))
        {
            return TokenType::LETTER;
        }
        else if (token.str[token.start] == '\\')
        {
            return TokenType::COMMAND;
        }
        else if (token.str[token.start] >= '0' && token.str[token.start] <= '9')
        {
            return TokenType::NUMBER;
        }
    }
    return TokenType::UNKNOWN;
}

struct PendingExpression
{
    Expression* expression;
    bool complete;
};

void complete_expression(Expression* lhs, Expression* rhs)
{
    switch (lhs->type)
    {
    case ExpressionType::BINARY_OP:
        if (lhs->binary_operation.y)
        {
            complete_expression(lhs->binary_operation.y, rhs);
        }
        else
        {
            lhs->binary_operation.y= rhs;
        }
        break;
    case ExpressionType::EXPONENT:
        if (lhs->exponent.rhs)
        {
            complete_expression(lhs->exponent.rhs, rhs);
        }
        else
        {
            lhs->exponent.rhs = rhs;
        }
        break;
    default:
        assert("expression cannot be completed");
    }
}

bool handle_token(SubStr token, PendingExpression* expr, int* num_expr)
{
    TokenType token_type = classify_token(token);

    PendingExpression current_expression;
    switch (token_type)
    {
    case TokenType::LETTER:
        { 
            ElementInfo* element = identify_element(token);
            if (element)
            {
                current_expression.expression = new_element(element);
                current_expression.complete = true;
            }
            else
            {
                std::cout << "unknown element" << std::endl;
                return false;
            }
        } break;
    case TokenType::NUMBER:
        current_expression.expression = new_element(&elements[0]);
        current_expression.complete = true;
        break;
    case TokenType::COMMAND:
        if (*num_expr != 0)
        {
            if (expr[*num_expr - 1].complete)
            {
                current_expression.expression =
                    new_binary_operation(&operations[0], expr[*num_expr - 1].expression, nullptr);
                current_expression.complete = false;

                (*num_expr)--;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        break;
    case TokenType::EXPONENT:
        if (*num_expr != 0)
        {
            if (expr[*num_expr - 1].complete)
            {
                current_expression.expression =
                    new_exponent(&operations[0], expr[*num_expr - 1].expression, nullptr);
                current_expression.complete = false;

                (*num_expr)--;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        break;
    default:
        return false;
    }
    
    if (*num_expr != 0)
    {
        if (expr[(*num_expr) - 1].complete)
        {
            // this is an implicit operation
            Expression* binary_operation = 
                new_binary_operation(
                        &operations[0],
                        expr[*num_expr - 1].expression,
                        current_expression.expression);
            expr[*num_expr - 1].expression = binary_operation;
            expr[*num_expr - 1].complete = true;
        }
        else
        {
            // we can complete this expression
            complete_expression(expr[*num_expr - 1].expression, current_expression.expression);
            expr[*num_expr - 1].complete = true;
        }
    }
    else
    {
        // we are the first expression
        expr[(*num_expr)++] = current_expression;
    }

    return true;
}

Expression* parse_latex(const char* latex)
{
    int num_expr = 0;
    PendingExpression expr[256];

    int start = 0;
    int token_length = next_token(latex, &start);
    while (token_length)
    {
        SubStr token;
        token.str = latex;
        token.start = start;
        token.end = start + token_length;

        if (!handle_token(token, expr, &num_expr))
        {
            return nullptr;
        }

        start = start + token_length;
        token_length = next_token(latex, &start);
    }

    if (num_expr != 1 || !expr[num_expr - 1].complete)
    {
        return nullptr;
    }

    return expr[0].expression;
}
