program TestPassByValue;

procedure Double(val1: Integer);
begin
    val1 *= 2;
end;

procedure Main();
var x:Integer;
begin
    x := 10;
    Double(x);
    WriteLn(x);
end;

begin
    Main();
end.
