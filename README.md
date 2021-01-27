# libaroma
an embedded ui toolkit  
  
## Some info
Libaroma is a graphics library wrote by Ahmad Amarullah (amarullz@xda-developers) almost entirely in C.  
It can be compiled for linux and some other OSes (like QNX, I didn't test that). Also has ARMv7/x86/MIPS optimizations, pretty useful.  
It has a very nice optimization, it runs very well on low-end and old devices like mine, an 850MHz single-core phone.  
This library has a lot of utilities, from drawing simple graphics (shapes, gradients, text and such) to an entire window managing system, and direct framebuffer drawing through canvases.  
 
## What changes I did?
I'm just trying to improve it, as the original one has some not-so-nice details (like the dialogs items being hardcoded in the library for the test program, instead of making those changeable by the user).  
So, if you want to know every change I did, then just look at the commit history (I've tried to keep it clear and concise by doing changes, testing them and the adding each change in a individual commit with a descriptive title/message, howerver didn't do that every time). 
A summary of changes: 
- Dialog items now are dynamic
- ~Window closing animation implemented~ (broken)
- Window now has appbar_bg property (ideal for setting statusbar color, if window has appbar)

## How to compile (Windows)
There are a lot of scripts for building libaroma on tools/win32, but you first need a toolchain. I use gcc-linaro-arm-linux-gnueabihf-4.7-2013.01-20130125_win32 from the launchpad's linaro archive (https://launchpad.net/linaro-toolchain-binaries/).
After that, the correct way to compile everything is this:
- go to tools/win32, edit envsetup.bat with the correct paths (you can just edit LIBAROMA_GCC and LIBAROMA_GPP variables), save and run it.
- Open a new CMD window (needed to make the envsetup work), and go to the tools/win32/libs then run makelibs.bat
- After that, go back (tools/win32) and run build_libaroma.bat
- You should have libaroma compiled and ready to build a test app! 
To compile the test app, you can use either deploy_recovery.bat template (or deploy_android.bat if your phone is on Android). Those scripts will compile the app, push it and run it on your device! **make sure to be rooted if you want to run it while on Android. Also, to close the app just press the power or home button**.  
If you just want to build the app, run build_app.bat template; the resulting binary will be at bin folder (tools/win32/bin)  
**To compile custom apps with the scripts**, they should have a folder inside examples\ with the name you want (like examples/template) and inside that a file called main.c (the app source code, look at template for an, ehm, template) and a folder called zip (with the resources you want to embed in the app like fonts, images, etc). If you want to make a flashable zip, just create META-INF/com/google/android folders inside zip and put there the resulting binary (rename it as update-binary). 

## How to use the build/deploy scripts
As you noticed before, there are three main scripts for building apps, and they parameters are the following: 
- build_app.bat nameoftheapp [f] (where nameoftheapp equals to the folder name on examples/, f is optional and skips the "press any key to continue" after building)
- deploy_android.bat nameoftheapp [r] (r is optional, it means don't build and just run into device)
- deploy_recovery.bat nameoftheapp [r] (same as above, but for recovery mode)

## Linux compiling?
Libaroma was prepared by Amarullz to be compiled on Windows, so linux build is under testing and may half-work (or won't work at all).  
Anyway, scripts here are a bit different from Windows ones: 
- envsetup.sh still needs to be edited with correct toolchain paths
- you still need to run tools/linux/libs/makelibs.sh in order to build dependencies
- libaroma.sh builds main library
- build.sh compiles libs, libaroma and the test app.
- "build_app.sh appname" compiles the app called "appname" and puts the binary at tools/linux/bin/appname
- "recovery.sh appname [r]" compiles the app and runs it on the recovery-mode connected device, if r is passed, it skips the compile process (just runs the app on the device).  
  
So, the main workflow to build libaroma and the test app on Linux is as follows:  
- Edit tools/linux/envsetup.sh with correct toolchain paths
- run either build.sh (to do everything automatically) or libs/makelibs.sh, libaroma.sh and then build_app.sh test
- to run the app on recovery, just use recovery.sh test r (if you want to compile the app and debug, you can omit the "r" at the end).  
## In progress
This is a list of changes I'm currently implementing (or didn't prepare for upload)
- [ ] Graphics composer (work in progress, troubles with transparency and related things)
- [ ] New stausbar example for window (need to be updated in order to work with composer)
- [ ] Slider control (in progress, need to make it a control)
 
For more info, please visit github page (it's outdated and not-so-useful, though): http://amarullz.github.io/libaroma/  
