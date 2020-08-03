# Palette
Palette is a [Concatenative Synthesizer](https://en.wikipedia.org/wiki/Concatenative_synthesis#In_music) in the making. 
I originally sat down to write a similar instrument called Nave. There are quite a few reasons for this rewrite which I do plan to enumerate
in a blog post. This project takes a lot of inspiration from CataRT. I was introduced to the Max program and instantly loved the concept, though
I believe there's a lot of room for improvement and a lot which could be gained by writing the software from scratch in C++.

# Building
Currently building is only tested and working on Windows. This is mostly because there's a resources folder containing sound files for testing which
needs to be located next to the executable. It should be trivial to remedy this in the future. 

To build, either generate a Visual Studio Solution from the Projucer file in the repository or 
simply open the Visual Studio Solution which is already committed to this repo.

Aside from JUCE, Palette relies on some external depencies. These include the libraries [fmt](https://github.com/fmtlib/fmt) and [range-v3](https://github.com/ericniebler/range-v3). These must be installed  on the system to work. On Windows, one can simply use vcpkg. To install these libraries with vcpkg, first install vcpkg then call:
`./vcpkg.exe --triplet x64-windows install fmt`
`./vcpkg.exe --triplet x64-windows install range-v3`

*Note that you must install the 64 bit versions of these libraries. vcpkg defaults to the 32 bit versions which won't be detected in our Visual Studio project.*