# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/haru/project/GitRepositoryManager/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/haru/project/GitRepositoryManager/build

# Include any dependencies generated for this target.
include CMakeFiles/grm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/grm.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/grm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/grm.dir/flags.make

CMakeFiles/grm.dir/grm.cpp.o: CMakeFiles/grm.dir/flags.make
CMakeFiles/grm.dir/grm.cpp.o: /home/haru/project/GitRepositoryManager/src/grm.cpp
CMakeFiles/grm.dir/grm.cpp.o: CMakeFiles/grm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/haru/project/GitRepositoryManager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/grm.dir/grm.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/grm.dir/grm.cpp.o -MF CMakeFiles/grm.dir/grm.cpp.o.d -o CMakeFiles/grm.dir/grm.cpp.o -c /home/haru/project/GitRepositoryManager/src/grm.cpp

CMakeFiles/grm.dir/grm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grm.dir/grm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/haru/project/GitRepositoryManager/src/grm.cpp > CMakeFiles/grm.dir/grm.cpp.i

CMakeFiles/grm.dir/grm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grm.dir/grm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/haru/project/GitRepositoryManager/src/grm.cpp -o CMakeFiles/grm.dir/grm.cpp.s

CMakeFiles/grm.dir/common.cpp.o: CMakeFiles/grm.dir/flags.make
CMakeFiles/grm.dir/common.cpp.o: /home/haru/project/GitRepositoryManager/src/common.cpp
CMakeFiles/grm.dir/common.cpp.o: CMakeFiles/grm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/haru/project/GitRepositoryManager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/grm.dir/common.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/grm.dir/common.cpp.o -MF CMakeFiles/grm.dir/common.cpp.o.d -o CMakeFiles/grm.dir/common.cpp.o -c /home/haru/project/GitRepositoryManager/src/common.cpp

CMakeFiles/grm.dir/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grm.dir/common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/haru/project/GitRepositoryManager/src/common.cpp > CMakeFiles/grm.dir/common.cpp.i

CMakeFiles/grm.dir/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grm.dir/common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/haru/project/GitRepositoryManager/src/common.cpp -o CMakeFiles/grm.dir/common.cpp.s

CMakeFiles/grm.dir/functions.cpp.o: CMakeFiles/grm.dir/flags.make
CMakeFiles/grm.dir/functions.cpp.o: /home/haru/project/GitRepositoryManager/src/functions.cpp
CMakeFiles/grm.dir/functions.cpp.o: CMakeFiles/grm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/haru/project/GitRepositoryManager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/grm.dir/functions.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/grm.dir/functions.cpp.o -MF CMakeFiles/grm.dir/functions.cpp.o.d -o CMakeFiles/grm.dir/functions.cpp.o -c /home/haru/project/GitRepositoryManager/src/functions.cpp

CMakeFiles/grm.dir/functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grm.dir/functions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/haru/project/GitRepositoryManager/src/functions.cpp > CMakeFiles/grm.dir/functions.cpp.i

CMakeFiles/grm.dir/functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grm.dir/functions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/haru/project/GitRepositoryManager/src/functions.cpp -o CMakeFiles/grm.dir/functions.cpp.s

# Object files for target grm
grm_OBJECTS = \
"CMakeFiles/grm.dir/grm.cpp.o" \
"CMakeFiles/grm.dir/common.cpp.o" \
"CMakeFiles/grm.dir/functions.cpp.o"

# External object files for target grm
grm_EXTERNAL_OBJECTS =

grm: CMakeFiles/grm.dir/grm.cpp.o
grm: CMakeFiles/grm.dir/common.cpp.o
grm: CMakeFiles/grm.dir/functions.cpp.o
grm: CMakeFiles/grm.dir/build.make
grm: CMakeFiles/grm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/haru/project/GitRepositoryManager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable grm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/grm.dir/build: grm
.PHONY : CMakeFiles/grm.dir/build

CMakeFiles/grm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/grm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/grm.dir/clean

CMakeFiles/grm.dir/depend:
	cd /home/haru/project/GitRepositoryManager/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/haru/project/GitRepositoryManager/src /home/haru/project/GitRepositoryManager/src /home/haru/project/GitRepositoryManager/build /home/haru/project/GitRepositoryManager/build /home/haru/project/GitRepositoryManager/build/CMakeFiles/grm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/grm.dir/depend
