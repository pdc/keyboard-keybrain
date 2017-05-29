Development
===========

My approach is to write components with unit tests (using [Google Test][])
using the standard C++ compiler (which is Clang on my computer), and then
install them as libraries in the [Arduino IDE][] for creating the integrated
code that runs on the microprocessor.

You can build and run the tests with

    make test

Or yoi can install [fswatch][] and do this to run the tests automatically as you edit the code:

    fswatch -o src/* tests/* | xargs -n1 -I{} make

To use the libraries in the IDE you need to find your Arduino libraries directory

  [Arduino IDE]: https://www.arduino.cc/en/main/software
  [Google Test]: ttps://github.com/google/googletest
  [fswatch]: https://github.com/emcrisostomo/fswatch
