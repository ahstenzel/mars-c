# Build the simple example in a Windows x64 MSVC environment
OUT = mygame
INC_DIR=..\..\..\include
LIB_DIR=..\..\..\bin\win\x64

all: $(OUT) clean

main.o : main.c
 @echo Compiling...
 cl /c /EHsc /I"$(INC_DIR)" /Fomain.o main.c

$(OUT) : main.o
 @echo Linking...
 link /OUT:$(OUT).exe /LIBPATH:"$(LIB_DIR)" main.o libmars.lib
 @copy $(LIB_DIR)\libmars.dll .\

clean:
 @echo Cleanup...
 del main.o