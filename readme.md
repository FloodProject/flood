Flood3D aims to be a modern, open-source 3D game engine and toolset.
It has been in development since 2008, with a focus on clean, testable,
modular, high-quality code. It is distributed under the BSD licence.

Building
========

All the dependencies are contained in the repository, and they need to
be built before building the rest of the code.

We use Premake as our build system, so you should be able to generate
project build files for your platform.

Visual Studio / Windows:

 1. Run deps/GenerateBuild.bat and generate the solution.
 2. Open the solution in deps/build/<vs> and build it.
 
 Remember to build all the configurations you're interested in,
 like 32-bit / 64-bit and Debug / Release.
 
 1. Run build/GenerateBuild.bat and generate the solution.
 2. Open the solution in build/<vs> and build it.
 

Other platforms build instructions will follow soon.