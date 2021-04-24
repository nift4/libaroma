@echo off
pushd %~dp0
mkdir libaroma\%LIBAROMA_ARCH%
cd libaroma\%LIBAROMA_ARCH%

if "%LIBAROMA_BUILD_DRM%"=="1" (
echo Building aroma minui wrapper

%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -lstdc++ ^
  -fPIC -DPIC ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
  %LIBAROMA_BASE%\src\contrib\platform\linux\aroma_minui.cpp ^
  -I%LIBAROMA_BASE%\libs\minui ^
  -I%LIBAROMA_BASE%\src\contrib\platform\linux\include
)

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
  ^
  %LIBAROMA_ARCH_OPTIMIZATIONS% ^
 ^
  %LIBAROMA_BASE%\src\contrib\platform\linux\fb_driver.c ^
  %LIBAROMA_BASE%\src\contrib\platform\linux\hid_driver.c ^
  %LIBAROMA_BASE%\src\contrib\platform\linux\platform.c ^
 ^
  %LIBAROMA_BASE%\src\aroma\aroma.c ^
  %LIBAROMA_BASE%\src\aroma\version.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_bar.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_button.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_fragment.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_image.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_label.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_list.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_pager.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_progress.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_scroll.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_tabs.c ^
  %LIBAROMA_BASE%\src\aroma\controls\ctl_toast.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_caption.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_check.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_divider.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_image.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_menu.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_option.c ^
  %LIBAROMA_BASE%\src\aroma\controls\listitem\listitem_text.c ^
  %LIBAROMA_BASE%\src\aroma\debug\compiler_message.c ^
  %LIBAROMA_BASE%\src\aroma\debug\memory_tracking.c ^
  %LIBAROMA_BASE%\src\aroma\graph\artworker.c ^
  %LIBAROMA_BASE%\src\aroma\graph\canvas.c ^
  %LIBAROMA_BASE%\src\aroma\graph\fb.c ^
  %LIBAROMA_BASE%\src\aroma\graph\image.c ^
  %LIBAROMA_BASE%\src\aroma\graph\jpeg.c ^
  %LIBAROMA_BASE%\src\aroma\graph\png.c ^
  %LIBAROMA_BASE%\src\aroma\graph\svg.c ^
  %LIBAROMA_BASE%\src\aroma\graph\text.c ^
  %LIBAROMA_BASE%\src\aroma\graph\draw\blur.c ^
  %LIBAROMA_BASE%\src\aroma\graph\draw\commondraw.c ^
  %LIBAROMA_BASE%\src\aroma\graph\draw\filters.c ^
  %LIBAROMA_BASE%\src\aroma\graph\draw\gradient.c ^
  %LIBAROMA_BASE%\src\aroma\graph\draw\path.c ^
  %LIBAROMA_BASE%\src\aroma\graph\draw\scale.c ^
  %LIBAROMA_BASE%\src\aroma\graph\engine\alignblt.c ^
  %LIBAROMA_BASE%\src\aroma\graph\engine\alpha.c ^
  %LIBAROMA_BASE%\src\aroma\graph\engine\blt.c ^
  %LIBAROMA_BASE%\src\aroma\graph\engine\color.c ^
  %LIBAROMA_BASE%\src\aroma\graph\engine\dither.c ^
  %LIBAROMA_BASE%\src\aroma\hid\hid.c ^
  %LIBAROMA_BASE%\src\aroma\hid\messages.c ^
  %LIBAROMA_BASE%\src\aroma\ui\color_manager.c ^
  %LIBAROMA_BASE%\src\aroma\ui\control.c ^
  %LIBAROMA_BASE%\src\aroma\ui\dialog.c ^
  %LIBAROMA_BASE%\src\aroma\ui\window.c ^
  %LIBAROMA_BASE%\src\aroma\ui\window_layer.c ^
  %LIBAROMA_BASE%\src\aroma\ui\window_manager.c ^
  %LIBAROMA_BASE%\src\aroma\utils\array.c ^
  %LIBAROMA_BASE%\src\aroma\utils\json.c ^
  %LIBAROMA_BASE%\src\aroma\utils\minutf8.c ^
  %LIBAROMA_BASE%\src\aroma\utils\motions.c ^
  %LIBAROMA_BASE%\src\aroma\utils\runtime.c ^
  %LIBAROMA_BASE%\src\aroma\utils\stream.c ^
  %LIBAROMA_BASE%\src\aroma\utils\strings.c ^
  %LIBAROMA_BASE%\src\aroma\utils\system.c ^
  %LIBAROMA_BASE%\src\aroma\utils\time.c ^
  %LIBAROMA_BASE%\src\aroma\utils\zip.c ^
 ^
  -I%LIBAROMA_BASE%\include ^
  -I%LIBAROMA_BASE%\src\contrib\platform\linux\include ^
  -I%LIBAROMA_BASE%\src ^
  -I%LIBAROMA_BASE%\libs\zlib\src ^
  -I%LIBAROMA_BASE%\libs\freetype\builds ^
  -I%LIBAROMA_BASE%\libs\freetype\include ^
  -I%LIBAROMA_BASE%\libs\minzip ^
  -I%LIBAROMA_BASE%\libs\selinux\include ^
  -I%LIBAROMA_BASE%\libs\png ^
  -I%LIBAROMA_BASE%\libs\jpeg ^
  -I%LIBAROMA_BASE%\libs\harfbuzz-ng\src ^
  -I%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ucdn 

cd ..
mkdir bin
popd
