mergeInto(LibraryManager.library, {
    init_math: function()
    {
        equation_pane = 
        {
            active_row: null,
        };

        equation_edit_callback = Module.cwrap('equation_edit_callback', null, ['string']);
    },
    begin_row: function()
    {
        equation_pane.active_row = document.createElement('mrow');
    },
    end_row: function()
    {
        var equations = document.getElementById('equation-pane');
        equations.appendChild(equation_pane.active_row);
        equation_pane.active_row = null;
    },
    display_math: function(latex_string)
    {
        katex.render(Pointer_stringify(latex_string), equation_pane.active_row);
    }
});
