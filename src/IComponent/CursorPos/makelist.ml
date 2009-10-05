<DEFAULT
COMPILE=gcc -g -c %SRCFILE% -o %MODULE%.%OBJ_EXT%
LPREFIX=gcc -o %EXEFILE%
LSUFFIX=-lbe -ltracker
OUTPUT=CursorPos

>>
CursorPos.cpp
../../IComm/ICommClient.cpp
../../../common/stat.cpp
<<
