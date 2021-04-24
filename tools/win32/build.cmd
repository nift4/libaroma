@ECHO OFF
pushd %~dp0
call clean.cmd
call libs\build_all.cmd
call libaroma.cmd
call app.cmd test
popd