# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /srv/jlrdragon92000/swsupport/idea/clion-2018.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /srv/jlrdragon92000/swsupport/idea/clion-2018.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/msh.X.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/msh.X.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/msh.X.dir/flags.make

CMakeFiles/msh.X.dir/msh.c.o: CMakeFiles/msh.X.dir/flags.make
CMakeFiles/msh.X.dir/msh.c.o: ../msh.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/msh.X.dir/msh.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/msh.X.dir/msh.c.o   -c "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/msh.c"

CMakeFiles/msh.X.dir/msh.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/msh.X.dir/msh.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/msh.c" > CMakeFiles/msh.X.dir/msh.c.i

CMakeFiles/msh.X.dir/msh.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/msh.X.dir/msh.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/msh.c" -o CMakeFiles/msh.X.dir/msh.c.s

CMakeFiles/msh.X.dir/msh.c.o.requires:

.PHONY : CMakeFiles/msh.X.dir/msh.c.o.requires

CMakeFiles/msh.X.dir/msh.c.o.provides: CMakeFiles/msh.X.dir/msh.c.o.requires
	$(MAKE) -f CMakeFiles/msh.X.dir/build.make CMakeFiles/msh.X.dir/msh.c.o.provides.build
.PHONY : CMakeFiles/msh.X.dir/msh.c.o.provides

CMakeFiles/msh.X.dir/msh.c.o.provides.build: CMakeFiles/msh.X.dir/msh.c.o


CMakeFiles/msh.X.dir/util.c.o: CMakeFiles/msh.X.dir/flags.make
CMakeFiles/msh.X.dir/util.c.o: ../util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/msh.X.dir/util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/msh.X.dir/util.c.o   -c "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/util.c"

CMakeFiles/msh.X.dir/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/msh.X.dir/util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/util.c" > CMakeFiles/msh.X.dir/util.c.i

CMakeFiles/msh.X.dir/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/msh.X.dir/util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/util.c" -o CMakeFiles/msh.X.dir/util.c.s

CMakeFiles/msh.X.dir/util.c.o.requires:

.PHONY : CMakeFiles/msh.X.dir/util.c.o.requires

CMakeFiles/msh.X.dir/util.c.o.provides: CMakeFiles/msh.X.dir/util.c.o.requires
	$(MAKE) -f CMakeFiles/msh.X.dir/build.make CMakeFiles/msh.X.dir/util.c.o.provides.build
.PHONY : CMakeFiles/msh.X.dir/util.c.o.provides

CMakeFiles/msh.X.dir/util.c.o.provides.build: CMakeFiles/msh.X.dir/util.c.o


CMakeFiles/msh.X.dir/jobs.c.o: CMakeFiles/msh.X.dir/flags.make
CMakeFiles/msh.X.dir/jobs.c.o: ../jobs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/msh.X.dir/jobs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/msh.X.dir/jobs.c.o   -c "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/jobs.c"

CMakeFiles/msh.X.dir/jobs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/msh.X.dir/jobs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/jobs.c" > CMakeFiles/msh.X.dir/jobs.c.i

CMakeFiles/msh.X.dir/jobs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/msh.X.dir/jobs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/jobs.c" -o CMakeFiles/msh.X.dir/jobs.c.s

CMakeFiles/msh.X.dir/jobs.c.o.requires:

.PHONY : CMakeFiles/msh.X.dir/jobs.c.o.requires

CMakeFiles/msh.X.dir/jobs.c.o.provides: CMakeFiles/msh.X.dir/jobs.c.o.requires
	$(MAKE) -f CMakeFiles/msh.X.dir/build.make CMakeFiles/msh.X.dir/jobs.c.o.provides.build
.PHONY : CMakeFiles/msh.X.dir/jobs.c.o.provides

CMakeFiles/msh.X.dir/jobs.c.o.provides.build: CMakeFiles/msh.X.dir/jobs.c.o


# Object files for target msh.X
msh_X_OBJECTS = \
"CMakeFiles/msh.X.dir/msh.c.o" \
"CMakeFiles/msh.X.dir/util.c.o" \
"CMakeFiles/msh.X.dir/jobs.c.o"

# External object files for target msh.X
msh_X_EXTERNAL_OBJECTS =

msh.X: CMakeFiles/msh.X.dir/msh.c.o
msh.X: CMakeFiles/msh.X.dir/util.c.o
msh.X: CMakeFiles/msh.X.dir/jobs.c.o
msh.X: CMakeFiles/msh.X.dir/build.make
msh.X: CMakeFiles/msh.X.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable msh.X"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/msh.X.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/msh.X.dir/build: msh.X

.PHONY : CMakeFiles/msh.X.dir/build

CMakeFiles/msh.X.dir/requires: CMakeFiles/msh.X.dir/msh.c.o.requires
CMakeFiles/msh.X.dir/requires: CMakeFiles/msh.X.dir/util.c.o.requires
CMakeFiles/msh.X.dir/requires: CMakeFiles/msh.X.dir/jobs.c.o.requires

.PHONY : CMakeFiles/msh.X.dir/requires

CMakeFiles/msh.X.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/msh.X.dir/cmake_clean.cmake
.PHONY : CMakeFiles/msh.X.dir/clean

CMakeFiles/msh.X.dir/depend:
	cd "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0" "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0" "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug" "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug" "/var/home/jlrdragon92000/Documents/AU18/C S 439 PRINCIPLES OF COMPUTER SYS-C S/proj0-wd/p0/cmake-build-debug/CMakeFiles/msh.X.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/msh.X.dir/depend
