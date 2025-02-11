@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"

echo Building Squirrel
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
  ^
  %LIBAROMA_CFLAGS% ^
	^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqapi.cpp ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqbaselib.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqclass.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqcompiler.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqdebug.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqfuncstate.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqlexer.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqmem.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqobject.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqstate.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqtable.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\squirrel\sqvm.cpp" ^
	^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdaux.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdblob.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdio.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdmath.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdrex.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdstream.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdstring.cpp" ^
	"%LIBAROMA_BASE%\libs\squirrel\sqstdlib\sqstdsystem.cpp" ^
	^
  -I"%LIBAROMA_BASE%\libs\squirrel\include" ^
  -I"%LIBAROMA_BASE%\libs\squirrel\squirrel" ^
  -I"%LIBAROMA_BASE%\libs\squirrel\sqstdlib"

popd
