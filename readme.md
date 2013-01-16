Flood3D aims to be a modern, open-source 3D game engine and toolset.
It has been in development since 2008, with a focus on clean, testable,
modular, high-quality code. It is distributed under the BSD licence.

Building
========

All the dependencies are contained in the repository, and they need to
be built before building the rest of the code.

We use Premake as our build system, so you should be able to generate
project build files for your platform.

### External Dependencies

Before trying to build, some dependencies need to be check out under
their respective folders in deps/.

1. Check out the Mono repository from https://github.com/mono/mono

## Visual Studio / Windows

Note: Build all the configurations you're interested in,
like 32-bit / 64-bit and Debug / Release versions.

1. Run `build/GenerateBuild.bat` and generate the solutions.
2. Customize the features in the generated `build/Config.lua`.
3. If you change anything in the config, run `build/GenerateConfig.bat`.
4. Open the Dependencies.sln solution in `build/<vs>` and build it.
5. Open the Flood.sln solution in `build/<vs>` and build it.

---

Other platforms build instructions will follow soon.
