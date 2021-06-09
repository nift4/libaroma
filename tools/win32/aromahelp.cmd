@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\tools\help\" (
	echo Help files not found^!
	goto :eof
)
if exist "%LIBAROMA_BASE%\tools\help\%1.txt" (
type "%LIBAROMA_BASE%\tools\help\%1.txt"
) else (
type "%LIBAROMA_BASE%\tools\help\aromahelp.txt"
)
