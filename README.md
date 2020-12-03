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

## In progress
This is a list of changes I'm currently implementing (or didn't prepare for upload)
- [ ] Template app, empty window with basic things to get started (in progress)
- [ ] Statusbar example for window (preparing for upload)
- [ ] Slider control (in progress, need to make it a control)
 
For more info, please visit github page (it's outdated and not-so-useful, though): http://amarullz.github.io/libaroma/  
