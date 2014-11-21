function [x0] = opt (x,y)
p=polyfit(x,y,2);
x0=-p(2)/p(1)/2;
endfunction
