# Palette
Palette is a [Concatenative Synthesizer](https://en.wikipedia.org/wiki/Concatenative_synthesis#In_music) in the making. 
I originally sat down to write a similar instrument called Nave. There are quite a few reasons for this rewrite which I do plan to enumerate
in a blog post. This project takes a lot of inspiration from CataRT. I was introduced to the Max program and instantly loved the concept, though
I believe there's a lot of room for improvement and a lot which could be gained by writing the software from scratch in C++.

# Building
Thanks to the FRUT project, build files can be generated with cmake. In Windows Powershell, we perform the following steps:

1. Clone with Dependencies
	`git clone https://github.com/BennetLeff/Palette --recursive`


2. Update Dependencies
	`git submodule update --init`

3. Build Dependencies
	
	Building FRUT:

	`cd Palette/dependencies/FRUT`
	`mkdir build`
	`cd build`
	`cmake .. -DCMAKE_INSTALL_PREFIX="../prefix" -DJUCE_ROOT="../../JUCE"`
	`cmake --build . --target install`

	Building fmt

	`cd ../../fmt`
	`mkdir build`
	`cd build`
	`cmake ..`
	`cmake --build . --target install`

4. Build Palette
	
	`cd ../../..`
	`mkdir build`
	`cd build`
	`cmake ..`

	Navigate to Palette/build in file explorer and open Palette.sln in Visual Studio.
	In Visual Studio, right-click either the Standalone or VST3 project in the list of projects and select Build.

5. Place Resources Folder
	
	The resources folder located at `Palette/resources` needs to be in the same directory as the executible in order for Palette to run properly. Copy `Palette/resources` and paste it into the output directory of your Visual Studio build.

	The executible should now run in your desired environment. 

The build system will be improved but for now this is the best option to get things up and running quickly. That's just C++ build systems for you.
