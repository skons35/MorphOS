# MorphOS
Find in *projects* some applications more oriented to MorphOS portability and tests.
One application at this time, to be completed.


## Introduction
The aim of the first *simpleSceneTester* project is testing set up and rendering of simple 3D scene using **OpenGL** / **GLUT** combination, keeping compatibility for most platforms.

Starting with usual Windows/Linux platforms, i will focus quickly on [MorphOS](https://fr.wikipedia.org/wiki/MorphOS) platform.

- On Windows/Linux platform, freeGlut & OpenGL libraries would be needed.
  Also, a **CMakeLists.txt** is provided to easily generate then project, using [CMake](https://cmake.org/)

- On MorphOS platform (typically PowerPC architecture, as my PowerBook  
  from ~2004), a **Makefile** will by provided for the project.

## Requirements
- On windows/linux, i suggest to install [VCPKG](https://github.com/microsoft/vcpkg)  
  first. Then install **opengl** and **freeglut** packages, with commands similar as the two below (example given for *windows* installation in *64 bits* flavour):
  ```
  vcpkg.exe install opengl --triplet x64-windows
  vcpkg.exe install freeglut --triplet x64-windows
  ```

- On MorphOS, a *TinyGL* library is providing some of the OpenGL/GlUT   
  functionnalities, possibly with lots of limitation or differences, i will try to update the code as needed. Also using the *make* build tools with provided *Makefile* will be the usual building approach.
  
## Starting steps
* clone this repository
```
git clone https://github.com/skons35/MorphOS.git
```

* For OpenGL / Glut tests project named : **simpleSceneTester**
* open command line, then go to the *projects* folder
```
cd projects
```
* (Windows/Linux) : create a **build** folder (where to generate project to be compiled) and go into it:
```
mkdir build
cd build
```
* (Windows/Linux) : Generate the project that links with OpenGL/GLUT libraries, 
so in the following call to cmake project generator, you'll need to provide valid *vckg* installation directory to let CMake retrieve installed dependancies 5see Windows example command below) :
```
cmake .. -DCMAKE_TOOLCHAIN_FILE=<PATH_TO_VCPKG_INSTALL_DIR>\scripts\buildsystems\vcpkg.cmake 

```

* (Windows/Linux) : ALTERNATIVE TO ABOVE STEP : you could also interactively adjust the settings or default values 
presented by CMakeLists.txt, and to do that, you may replace the command from previous step 
by the more user friendly one (allowing UI interaction) :
```
cmake-gui ..
```
	- Then do the required changes when needed ( then click on **Configure**, any remaining errors will stay as RED lines display). Typically here you should adjust/assign the path to find a valid OpenGL/(Free)Glut install folder (if using vckpg, it will be below its install folder)
	- Finally click on **Generate** to generate the project files with defined settings. 
	  The message *Generating done* should be displayed.
	- You may quit the cmake-gui interface.

* Compile the generated project (for instance, if Visual Studio was the compiling IDE defined,
 you should find a simpleSceneTester.sln_ file, that you can open with that IDE, and then compile the project.
* launch / test the generated binary.

**SimpleSceneTester** is a very simple OpenGL program, that will aims to load and display
 an OBJ file ( a default one, or a specific one if filename is provided as command line parameter)
It can be launched using samples scripts in  **tests_scripts** folder.
I also provide the sample data (OBJ files and texture in **data** folder).
User may trigger object slow rotation by using mouse buttons (Left is starting rotation,
Right is inverting rotation, middle is stopping the auto-rotation.) or move forward, backward 
from the object using mouse scrolling button.

Some dedicated BMP code load test added
Some dedicated MTL code load test added

*Note 1:*
Only uncompressed BMP texture file(s) are managed at this time (more to be added later).

*Note 2:*
Textured objects are correctly textured/rendered ONLY if NO vertex are shared amongst defined faces, 
and each face must be a triangle (not a quad or polygon). This limitation comes from the use of GlDrawElement() 
with vertices indices arrays combined to texture array use. This limitation will be removed later, for the moment 
loaded objects must follow that quite restrictive limitation (but purely color based objects have not such limitation and 
are correclty rendered).
