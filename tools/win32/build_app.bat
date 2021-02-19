@ECHO OFF
cls
cd libaroma

echo Compiling %1
%LIBAROMA_GCC% ^
  -static ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -w -Wl,-s ^
  ^
  %LIBAROMA_CFLAGS% ^
  ^
    -DLIBAROMA_CONFIG_DEBUG=%LIBAROMA_CONFIG_DEBUG% ^
    -DLIBAROMA_CONFIG_DEBUG_FILE=%LIBAROMA_CONFIG_DEBUG_FILE% ^
    -DLIBAROMA_CONFIG_DEBUG_MEMORY=%LIBAROMA_CONFIG_DEBUG_MEMORY% ^
    -DLIBAROMA_CONFIG_COMPILER_MESSAGE=%LIBAROMA_CONFIG_COMPILER_MESSAGE% ^
    -DLIBAROMA_CONFIG_SHMEMFB=%LIBAROMA_CONFIG_SHMEMFB% ^
  ^
    ../obj/*.o ./*.o ^
    ../../../examples/%1/*.c ^
  ^
  -I../../../include ^
  -I../../../libs/selinux/include ^
  -I../../../src ^
  -I../../../src %LIBAROMA_CINCLUDES% ^
  -o ../bin/%1 ^
  ^
  -ldrm -lm -lpthread -lstdc++ -lrt

cd ..
