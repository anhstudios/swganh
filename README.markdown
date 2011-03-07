[swganh][1] - an open source Star Wars Galaxies Publish 14.1 server emulator
=======================================================================

This project aims to recreate the Star Wars Galaxies Pre-Combat Upgrade experience as it was during Publish 14.1. In addition it serves as a playground for experimenting with new ideas for MMORPG server architecture, for more information see the [online documentation][2].

## Quick Build Guide ##

### Requirements ###

*   Microsoft Visual Studio 10.0 or higher (windows)
*   GCC 4.6 (*nix)
*   CMake 2.8.3 or higher
*   Git 1.7 or higher

First, check out the project completely with the following, Windows users should run these commands from the Git Bash utility:

    git clone https://github.com/anhstudios/swganh.git
    cd swganh
    git submodule update --init
    
After checking out the source you can perform the initial build by running provided bootstrap shell script:

    ./bootstrap.sh
    
This process can take anywhere from 15-40 minutes on the first build depending on the hardware used. Generated binaries can be found in the **/build/bin** directory.

### Visual Studio / Windows Development ###

On windows the platform of choice is Visual Studio 2010 or better. While other compilers such as GCC 4.6 via cygwin is possible it's hard to top the Visual Studio IDE for development. To get started, after you've run the bootstrap script as outlined in the Quick Build Guide, open the **/build/swganh/swganh.sln** solution file.

* **NOTE** Since the project files are located outside the source directory adding new files from within visual studio requires changing the default save location.*

### GCC / Linux Development ###

On linux platforms a minimum of GCC 4.6 is required to build the project. After running the bootstrap script as outlined in the Quick Build Guide change directories to the **build** directory. You can use your favorite text editor to edit source files, the project can be rebuilt using familiar gnu make commands.

    cd build
    cmake ..
    make

### Clang and other compilers ###

[Clang][3] does not yet support lambdas, a C++0x feature commonly used in this project. Once lambda support is available we will investigate officially adopting it as a supported compiler. 

Intel's C++ compiler is supported with [Intel Parallel Studio 2011][4] or greater.

## Quick Links ##

*   **[Online Documentation][2]**

[1]: http://swganh.com
[2]: http://swganh.com/docs
[3]: http://clang.llvm.org/
[4]: http://software.intel.com/en-us/articles/intel-parallel-studio-home/

