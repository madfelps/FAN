# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /snap/clion/152/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/152/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/felipe/CLionProjects/FAN

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/felipe/CLionProjects/FAN/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FAN.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FAN.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FAN.dir/flags.make

CMakeFiles/FAN.dir/mainn.cpp.o: CMakeFiles/FAN.dir/flags.make
CMakeFiles/FAN.dir/mainn.cpp.o: ../mainn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/felipe/CLionProjects/FAN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FAN.dir/mainn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FAN.dir/mainn.cpp.o -c /home/felipe/CLionProjects/FAN/mainn.cpp

CMakeFiles/FAN.dir/mainn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FAN.dir/mainn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/felipe/CLionProjects/FAN/mainn.cpp > CMakeFiles/FAN.dir/mainn.cpp.i

CMakeFiles/FAN.dir/mainn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FAN.dir/mainn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/felipe/CLionProjects/FAN/mainn.cpp -o CMakeFiles/FAN.dir/mainn.cpp.s

CMakeFiles/FAN.dir/RemyHVH250.cpp.o: CMakeFiles/FAN.dir/flags.make
CMakeFiles/FAN.dir/RemyHVH250.cpp.o: ../RemyHVH250.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/felipe/CLionProjects/FAN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FAN.dir/RemyHVH250.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FAN.dir/RemyHVH250.cpp.o -c /home/felipe/CLionProjects/FAN/RemyHVH250.cpp

CMakeFiles/FAN.dir/RemyHVH250.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FAN.dir/RemyHVH250.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/felipe/CLionProjects/FAN/RemyHVH250.cpp > CMakeFiles/FAN.dir/RemyHVH250.cpp.i

CMakeFiles/FAN.dir/RemyHVH250.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FAN.dir/RemyHVH250.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/felipe/CLionProjects/FAN/RemyHVH250.cpp -o CMakeFiles/FAN.dir/RemyHVH250.cpp.s

# Object files for target FAN
FAN_OBJECTS = \
"CMakeFiles/FAN.dir/mainn.cpp.o" \
"CMakeFiles/FAN.dir/RemyHVH250.cpp.o"

# External object files for target FAN
FAN_EXTERNAL_OBJECTS =

FAN: CMakeFiles/FAN.dir/mainn.cpp.o
FAN: CMakeFiles/FAN.dir/RemyHVH250.cpp.o
FAN: CMakeFiles/FAN.dir/build.make
FAN: CMakeFiles/FAN.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/felipe/CLionProjects/FAN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable FAN"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FAN.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FAN.dir/build: FAN

.PHONY : CMakeFiles/FAN.dir/build

CMakeFiles/FAN.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FAN.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FAN.dir/clean

CMakeFiles/FAN.dir/depend:
	cd /home/felipe/CLionProjects/FAN/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/felipe/CLionProjects/FAN /home/felipe/CLionProjects/FAN /home/felipe/CLionProjects/FAN/cmake-build-debug /home/felipe/CLionProjects/FAN/cmake-build-debug /home/felipe/CLionProjects/FAN/cmake-build-debug/CMakeFiles/FAN.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FAN.dir/depend

