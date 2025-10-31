## ABout project ##

This is a simple program managed with GNU Autotools. It prints a "Hello, World!" message using a separate function.

## Build and Install ##

First, you need to generate the build system scripts, for this follow this command:

autoreconf -i

Next, run the configure script:

./configure

Now, you can build the project using the make command:

make

To install the program and its header file onto your system, run the following command:

sudo make install

## Run and clean ##

To run the project, execute this command:

./helloworld

To clean the project directory and remove all files generated during the build process, as well as uninstall the program from your system, use this command:

make clean-all
