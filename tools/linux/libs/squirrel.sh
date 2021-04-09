#!/bin/sh
mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/squirrel
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/squirrel

echo Compiling Squirrel
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s \
 \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqapi.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqbaselib.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqclass.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqcompiler.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqdebug.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqfuncstate.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqlexer.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqmem.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqobject.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqstate.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqtable.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/squirrel/sqvm.cpp \
 \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdaux.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdblob.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdio.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdmath.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdrex.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdstream.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdstring.cpp \
  ${LIBAROMA_BASE}/libs/squirrel/sqstdlib/sqstdsystem.cpp \
 \
  -I${LIBAROMA_BASE}/libs/squirrel/include \
  -I${LIBAROMA_BASE}/libs/squirrel/squirrel \
  -I${LIBAROMA_BASE}/libs/squirrel/sqstdlib

cd ${LIBAROMA_BASE}/tools/linux/libs
