function get_new_expr_key()
{
    var key = 0;
    while (key in equation_pane.expressions)
    {
        key += 1;
    }
    return key;
}
