
{_,FL} = file:list_dir(".").
lists:filter(fun(A)->filelib:is_file(A) end,FL)
