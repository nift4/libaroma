@echo off
mkdir libaroma
cd libaroma

echo Building aroma minui wrapper

%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -lstdc++ ^
  -fPIC -DPIC ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
  ..\..\..\src\contrib\platform\linux\aroma_minui.cpp ^
  -I..\..\..\libs\minui ^
  -I..\..\..\src\contrib\platform\linux\include

echo Building libaroma
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -lstdc++ %LIBAROMA_STRIP_OBJECT% ^
  -fPIC -DPIC ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
  -DLIBAROMA_CONFIG_DEBUG=%LIBAROMA_CONFIG_DEBUG% ^
  -DLIBAROMA_CONFIG_DEBUG_FILE=%LIBAROMA_CONFIG_DEBUG_FILE% ^
  -DLIBAROMA_CONFIG_DEBUG_MEMORY=%LIBAROMA_CONFIG_DEBUG_MEMORY% ^
  -DLIBAROMA_CONFIG_COMPILER_MESSAGE=%LIBAROMA_CONFIG_COMPILER_MESSAGE% ^
  -DLIBAROMA_CONFIG_SHMEMFB=%LIBAROMA_CONFIG_SHMEMFB% ^
  -DANDROID=1 -D__ANDROID__ ^
 ^
  ..\..\..\src\contrib\platform\linux\fb_driver.c ^
  ..\..\..\src\contrib\platform\linux\hid_driver.c ^
  ..\..\..\src\contrib\platform\linux\platform.c ^
 ^
  ..\..\..\src\aroma\aroma.c ^
  ..\..\..\src\aroma\version.c ^
  ..\..\..\src\aroma\controls\ctl_bar.c ^
  ..\..\..\src\aroma\controls\ctl_button.c ^
  ..\..\..\src\aroma\controls\ctl_fragment.c ^
  ..\..\..\src\aroma\controls\ctl_image.c ^
  ..\..\..\src\aroma\controls\ctl_label.c ^
  ..\..\..\src\aroma\controls\ctl_list.c ^
  ..\..\..\src\aroma\controls\ctl_pager.c ^
  ..\..\..\src\aroma\controls\ctl_progress.c ^
  ..\..\..\src\aroma\controls\ctl_scroll.c ^
  ..\..\..\src\aroma\controls\ctl_tabs.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_caption.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_check.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_divider.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_image.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_menu.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_option.c ^
  ..\..\..\src\aroma\controls\listitem\listitem_text.c ^
  ..\..\..\src\aroma\debug\compiler_message.c ^
  ..\..\..\src\aroma\debug\memory_tracking.c ^
  ..\..\..\src\aroma\graph\artworker.c ^
  ..\..\..\src\aroma\graph\canvas.c ^
  ..\..\..\src\aroma\graph\fb.c ^
  ..\..\..\src\aroma\graph\image.c ^
  ..\..\..\src\aroma\graph\jpeg.c ^
  ..\..\..\src\aroma\graph\png.c ^
  ..\..\..\src\aroma\graph\svg.c ^
  ..\..\..\src\aroma\graph\text.c ^
  ..\..\..\src\aroma\graph\draw\blur.c ^
  ..\..\..\src\aroma\graph\draw\commondraw.c ^
  ..\..\..\src\aroma\graph\draw\filters.c ^
  ..\..\..\src\aroma\graph\draw\gradient.c ^
  ..\..\..\src\aroma\graph\draw\path.c ^
  ..\..\..\src\aroma\graph\draw\scale.c ^
  ..\..\..\src\aroma\graph\engine\alignblt.c ^
  ..\..\..\src\aroma\graph\engine\alpha.c ^
  ..\..\..\src\aroma\graph\engine\blt.c ^
  ..\..\..\src\aroma\graph\engine\color.c ^
  ..\..\..\src\aroma\graph\engine\dither.c ^
  ..\..\..\src\aroma\hid\hid.c ^
  ..\..\..\src\aroma\hid\messages.c ^
  ..\..\..\src\aroma\ui\color_manager.c ^
  ..\..\..\src\aroma\ui\control.c ^
  ..\..\..\src\aroma\ui\dialog.c ^
  ..\..\..\src\aroma\ui\window.c ^
  ..\..\..\src\aroma\ui\window_layer.c ^
  ..\..\..\src\aroma\ui\window_manager.c ^
  ..\..\..\src\aroma\utils\array.c ^
  ..\..\..\src\aroma\utils\json.c ^
  ..\..\..\src\aroma\utils\minutf8.c ^
  ..\..\..\src\aroma\utils\motions.c ^
  ..\..\..\src\aroma\utils\runtime.c ^
  ..\..\..\src\aroma\utils\stream.c ^
  ..\..\..\src\aroma\utils\strings.c ^
  ..\..\..\src\aroma\utils\system.c ^
  ..\..\..\src\aroma\utils\time.c ^
  ..\..\..\src\aroma\utils\zip.c ^
 ^
  -I..\..\..\include ^
  -I..\..\..\src\contrib\platform\linux\include ^
  -I..\..\..\src ^
  -I..\..\..\libs\zlib\src ^
  -I..\..\..\libs\freetype\builds ^
  -I..\..\..\libs\freetype\include ^
  -I..\..\..\libs\minzip ^
  -I..\..\..\libs\selinux\include ^
  -I..\..\..\libs\png ^
  -I..\..\..\libs\jpeg ^
  -I..\..\..\libs\harfbuzz-ng\src ^
  -I..\..\..\libs\harfbuzz-ng\src\hb-ucdn 

cd ..
mkdir bin
