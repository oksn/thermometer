
for /R %%f in ( *.cpp;*.c;*.h ) do astyle --style=ansi -s4 -S -N -L -m0 -M40 -p -P --convert-tabs --suffix=.pre "%%f"