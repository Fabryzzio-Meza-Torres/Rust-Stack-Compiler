program ExampleProgram;

//Para poder usar variables locales en Pascal, es necesario declararlas dentro
//de la sección var. Esto solo es posible en procedimientos y funciones.
// La mejor forma de evaluarles a los chicos variables locales y globales
// con pascal es con funciones
var
    x: Integer;
    i: Integer;
begin
    x := 1;
    for i := 0 to 9 do
    begin
        x := x + i;
    end;

    writeln(x);
end.
