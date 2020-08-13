# Palette
Palette is a [Concatenative Synthesizer](https://en.wikipedia.org/wiki/Concatenative_synthesis#In_music) in the making. 
I originally sat down to write a similar instrument called Nave. There are quite a few reasons for this rewrite which I do plan to enumerate
in a blog post. This project takes a lot of inspiration from CataRT. I was introduced to the Max program and instantly loved the concept, though
I believe there's a lot of room for improvement and a lot which could be gained by writing the software from scratch in C++.

# Building
Thanks to the FRUT project, build files can be generated with cmake.
To generate build files, run `mkdir build && cd build` then run `cmake .. `

Finally, please copy the `resources` folder to the same directory that the Palette executable is created at. 
The build system will be improved but for now this is the best option to get things up and running quickly. That's just C++ build systems for you.
