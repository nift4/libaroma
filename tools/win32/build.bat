@ECHO OFF
cd libs
call build_all.bat
cd ..
echo.
call libaroma.bat
echo.
call app.bat test
echo.