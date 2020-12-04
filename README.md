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
- Window closing animation implemented
- Window now has appbar_bg property (ideal for setting statusbar color, if window has appbar)

## How to compile (Windows)
There are a lot of scripts for building libaroma on tools/win32, but you first need a toolchain. I use 
After that, the correct way to compile everything is this:
- go to tools/win32, edit envsetup.bat with the correct paths (you can just edit LBIAROMA_GCC and LIBAROMA_GPP variables), save and run it.
- Open a new CMD window (needed to make the envsetup work), and go to the tools/win32/libs then run makelibs.bat
- After that, go back (tools/win32) and run build_libaroma.bat
- You should have libaroma compiled and ready to build a test app! 
To compile the test app, you can use either deploy_recovery.bat template (or deploy_android.bat if your phone is on Android). Those scripts will compile the app, push it and run it on your device! **make sure to be rooted if you awnt to run it while on Android. Also, to close the app just press the power or home button**.  
If you just want to build the app, run build_app.bat template; the resulting binary will be at bin folder (tools/win32/bin)  
**To compile custom apps with the scripts**, they should have a folder inside examples\ with the name you want (like examples/template) and inside that a file called main.c (the app source code, look at template for an, ehm, template) and a folder called zip (with the resources you want to embed in the app like fonts, images, etc). If you want to make a flashable zip, just create META-INF/com/google/android folders inside zip and put there the resulting binary (rename it as update-binary). 

## How to use the build/deploy scripts
As you noticed before, there are three main scripts for building apps, and they parameters are the following: 
- build_app.bat nameoftheapp [f] (where nameoftheapp equals to the folder name on examples/, f is optional and skips the "press any key to continue" after building)
- deploy_android.bat nameoftheapp [r] (r is optional, it means don't build and just run into device)
- deploy_recovery.bat nameoftheapp [r] (same as above, but for recovery mode)

## Linux compiling?
If you checked the tools tree, you may notice that win32 folder has a lot of things more than linux ones, and that's because Ahmad ever compiled libaroma under windows. I've tried compiling it on Linux, and while the build scripts were outdated they did work (after updating them).  
Anyway, I don't recommend compiling under Linux, because it has a lot of troubles (maybe it was my toolchain?). It can't use OMP (one library for better performance and other things), the sidebar doesn't work and libaroma in general has pretty strange behavior (if text in a control is more than 10/15 characters it may segfault; also if you draw a gradient on the window background the list background gets drawn too). Oh, and for some reason when I tried to create a menu listitem with the word "Wipe" that segfaulted too. 
If you can test wether Linux compiling does work correctly or not, I would be grateful to receive a report of any try.

## In progress
This is a list of changes I'm currently implementing (or didn't prepare for upload)
- [ ] Template app, empty window with basic things to get started (in progress)
- [ ] Statusbar example for window (preparing for upload)
- [ ] Slider control (in progress, need to make it a control)
 
For more info, please visit github page (it's outdated and not-so-useful, though): http://amarullz.github.io/libaroma/  
