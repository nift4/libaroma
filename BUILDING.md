# Building libaroma 

Unlike most libraries, this one comes with it's own build system instead of makefiles. Writing makefiles is something expected later, but right now I have no time to do so.

## dependencies

This library includes all of it's dependencies already bundled under libs/ directory, and they're built at compile time.
Optionally (and if you're building on Linux), you can set SDL as target to test your apps directly on your build machine (you need to install libsdl development headers and library).

## configuration

depending on the system you build on (Linux or Windows), there are two configuration files:

config-win.txt - configuration used while building on Windows
config-linux.txt - configuration used while building on Linux

The minimum options that a configuration file should set are: (read the config files included for examples)

LIBAROMA_TOOLCHAIN_PREFIX - sets a path (with prefix) to the toolchain (compiler) you will use for building
LIBAROMA_ARCH - the target platform's processor architecture (default is ARM)
LIBAROMA_PLATFORM - the target platform which you're building for (default is Linux)

Optionally you can set some other options, just read the config files for more options and what they do.

## preparation

run the envsetup file according to the system you're using to build: 
NOTE: all commands mentioned later need to be executed _**without**_ the double quotes.

windows - run envsetup.cmd (either double click or call it from a cmd command prompt)
linux - in main repository folder, open a terminal and run ". envsetup.sh" (may also work with "source envsetup.sh")

You should see "Ready to build libaroma-release targetting [architecture]".
Congrats! Now your console has special commands and you're ready to build libaroma apps :)

## new commands

Some of the special commands added after running envsetup are:

aromahelp - list special commands and get help for them
mkaroma - build aroma library and, if they were not built, it's dependencies.
mkapp - build aroma apps located at apps/[appname]

There are more commands, just use aromahelp to get a complete list (or aromahelp commandname for help about a specific command)

## building libaroma and dependencies

In order to start developing apps with libaroma, you need to compile it first.
Just run "mkaroma" and it should start building dependencies (as they weren't built first).

## building a test application

To ensure that everything was correctly set up, you may want to build a test app before starting the development of your own. 
By running "mkapp test", the build environment will start building the "test" app (whose code is at apps/test/). 
If everything goes well, you should get a file called "test-[architecture]-release" at out/bin/. If that's not the case, don't worry! You can always open an issue or email me (like I've said on the main readme). I'm not always available, but you may get help from others if you open an issue :D

## cleaning output

If you want to clean the previously built files (either for aroma, it's dependencies or everything) you have the **aromaclean** command. 
Just running "aromaclean" will delete aroma and dependencies files built for the current architecture.
You can optionally pass "all" as argument for it to clean everything inside out/libs/ (because you can build libaroma for more than a single architecture), or even run "aromaclean [architecture]" (passing the architecture that you want to clean as argument) for it to delete built files for a specific architecture.