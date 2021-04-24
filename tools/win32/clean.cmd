pushd %~dp0
if "%1" == "all" (
echo Clearing built files for all targets
if exist libaroma\ del /F /Q libaroma
if exist obj\ del /F /Q obj
mkdir libaroma
mkdir obj
) else (
echo Clearing built files for %LIBAROMA_ARCH%
if exist libaroma\%LIBAROMA_ARCH%\ del /F /Q libaroma\%LIBAROMA_ARCH%
if exist obj\%LIBAROMA_ARCH%\ del /F /Q obj\%LIBAROMA_ARCH%
mkdir libaroma\%LIBAROMA_ARCH%
mkdir obj\%LIBAROMA_ARCH%
)
popd