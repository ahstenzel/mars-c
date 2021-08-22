# Build the simple example in a Windows x86 MSVC environment
OUT = mygame
INC_DIR=C:\Program Files\Mars\include
LIB_DIR=C:\Program Files\Mars\lib

all: $(OUT) clean

main.o : main.c
 @echo Compiling...
 cl /c /EHsc /I"$(INC_DIR)" /Fomain.o main.c

$(OUT) : main.o
 @echo Linking...
 link /OUT:$(OUT).exe /LIBPATH:"$(LIB_DIR)" main.o libmars.lib

clean:
 @echo Cleanup...
 del main.o