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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shang/code/C++/github-project/student-work-project/tinyrpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shang/code/C++/github-project/student-work-project/tinyrpc/build

# Include any dependencies generated for this target.
include example/server/CMakeFiles/provider.dir/depend.make

# Include the progress variables for this target.
include example/server/CMakeFiles/provider.dir/progress.make

# Include the compile flags for this target's objects.
include example/server/CMakeFiles/provider.dir/flags.make

example/server/CMakeFiles/provider.dir/user_service.cc.o: example/server/CMakeFiles/provider.dir/flags.make
example/server/CMakeFiles/provider.dir/user_service.cc.o: ../example/server/user_service.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shang/code/C++/github-project/student-work-project/tinyrpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/server/CMakeFiles/provider.dir/user_service.cc.o"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && /usr/bin/x86_64-linux-gnu-g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/provider.dir/user_service.cc.o -c /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/server/user_service.cc

example/server/CMakeFiles/provider.dir/user_service.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/user_service.cc.i"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && /usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/server/user_service.cc > CMakeFiles/provider.dir/user_service.cc.i

example/server/CMakeFiles/provider.dir/user_service.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/user_service.cc.s"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && /usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/server/user_service.cc -o CMakeFiles/provider.dir/user_service.cc.s

example/server/CMakeFiles/provider.dir/user_service.cc.o.requires:

.PHONY : example/server/CMakeFiles/provider.dir/user_service.cc.o.requires

example/server/CMakeFiles/provider.dir/user_service.cc.o.provides: example/server/CMakeFiles/provider.dir/user_service.cc.o.requires
	$(MAKE) -f example/server/CMakeFiles/provider.dir/build.make example/server/CMakeFiles/provider.dir/user_service.cc.o.provides.build
.PHONY : example/server/CMakeFiles/provider.dir/user_service.cc.o.provides

example/server/CMakeFiles/provider.dir/user_service.cc.o.provides.build: example/server/CMakeFiles/provider.dir/user_service.cc.o


example/server/CMakeFiles/provider.dir/__/user.pb.cc.o: example/server/CMakeFiles/provider.dir/flags.make
example/server/CMakeFiles/provider.dir/__/user.pb.cc.o: ../example/user.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shang/code/C++/github-project/student-work-project/tinyrpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object example/server/CMakeFiles/provider.dir/__/user.pb.cc.o"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && /usr/bin/x86_64-linux-gnu-g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/provider.dir/__/user.pb.cc.o -c /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/user.pb.cc

example/server/CMakeFiles/provider.dir/__/user.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/__/user.pb.cc.i"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && /usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/user.pb.cc > CMakeFiles/provider.dir/__/user.pb.cc.i

example/server/CMakeFiles/provider.dir/__/user.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/__/user.pb.cc.s"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && /usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/user.pb.cc -o CMakeFiles/provider.dir/__/user.pb.cc.s

example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.requires:

.PHONY : example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.requires

example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.provides: example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.requires
	$(MAKE) -f example/server/CMakeFiles/provider.dir/build.make example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.provides.build
.PHONY : example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.provides

example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.provides.build: example/server/CMakeFiles/provider.dir/__/user.pb.cc.o


# Object files for target provider
provider_OBJECTS = \
"CMakeFiles/provider.dir/user_service.cc.o" \
"CMakeFiles/provider.dir/__/user.pb.cc.o"

# External object files for target provider
provider_EXTERNAL_OBJECTS =

../bin/provider: example/server/CMakeFiles/provider.dir/user_service.cc.o
../bin/provider: example/server/CMakeFiles/provider.dir/__/user.pb.cc.o
../bin/provider: example/server/CMakeFiles/provider.dir/build.make
../bin/provider: ../lib/libmprpc.a
../bin/provider: example/server/CMakeFiles/provider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shang/code/C++/github-project/student-work-project/tinyrpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../bin/provider"
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/provider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/server/CMakeFiles/provider.dir/build: ../bin/provider

.PHONY : example/server/CMakeFiles/provider.dir/build

example/server/CMakeFiles/provider.dir/requires: example/server/CMakeFiles/provider.dir/user_service.cc.o.requires
example/server/CMakeFiles/provider.dir/requires: example/server/CMakeFiles/provider.dir/__/user.pb.cc.o.requires

.PHONY : example/server/CMakeFiles/provider.dir/requires

example/server/CMakeFiles/provider.dir/clean:
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server && $(CMAKE_COMMAND) -P CMakeFiles/provider.dir/cmake_clean.cmake
.PHONY : example/server/CMakeFiles/provider.dir/clean

example/server/CMakeFiles/provider.dir/depend:
	cd /home/shang/code/C++/github-project/student-work-project/tinyrpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shang/code/C++/github-project/student-work-project/tinyrpc /home/shang/code/C++/github-project/student-work-project/tinyrpc/example/server /home/shang/code/C++/github-project/student-work-project/tinyrpc/build /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server /home/shang/code/C++/github-project/student-work-project/tinyrpc/build/example/server/CMakeFiles/provider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/server/CMakeFiles/provider.dir/depend

