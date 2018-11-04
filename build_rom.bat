
set OBJ=obj
set ROM=rom
set EMU=bgb

if "%1"=="clean" (
	if exist %OBJ% rd /s/q %OBJ%
	if exist %ROM% rd /s/q %ROM%
)

if not exist %OBJ% mkdir %OBJ%
if not exist %ROM% mkdir %ROM%
call make
move .\gbghost.gb %ROM%\gbghost.gb
call make clean
call %EMU%\bgb %ROM%\gbghost.gb

:end