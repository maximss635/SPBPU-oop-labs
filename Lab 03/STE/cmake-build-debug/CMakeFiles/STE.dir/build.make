# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/STE.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/STE.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/STE.dir/flags.make

CMakeFiles/STE.dir/main.cpp.o: CMakeFiles/STE.dir/flags.make
CMakeFiles/STE.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/STE.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STE.dir/main.cpp.o -c "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/main.cpp"

CMakeFiles/STE.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STE.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/main.cpp" > CMakeFiles/STE.dir/main.cpp.i

CMakeFiles/STE.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STE.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/main.cpp" -o CMakeFiles/STE.dir/main.cpp.s

CMakeFiles/STE.dir/Console.cpp.o: CMakeFiles/STE.dir/flags.make
CMakeFiles/STE.dir/Console.cpp.o: ../Console.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/STE.dir/Console.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STE.dir/Console.cpp.o -c "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/Console.cpp"

CMakeFiles/STE.dir/Console.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STE.dir/Console.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/Console.cpp" > CMakeFiles/STE.dir/Console.cpp.i

CMakeFiles/STE.dir/Console.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STE.dir/Console.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/Console.cpp" -o CMakeFiles/STE.dir/Console.cpp.s

CMakeFiles/STE.dir/MyString.cpp.o: CMakeFiles/STE.dir/flags.make
CMakeFiles/STE.dir/MyString.cpp.o: ../MyString.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/STE.dir/MyString.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STE.dir/MyString.cpp.o -c "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/MyString.cpp"

CMakeFiles/STE.dir/MyString.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STE.dir/MyString.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/MyString.cpp" > CMakeFiles/STE.dir/MyString.cpp.i

CMakeFiles/STE.dir/MyString.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STE.dir/MyString.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/MyString.cpp" -o CMakeFiles/STE.dir/MyString.cpp.s

# Object files for target STE
STE_OBJECTS = \
"CMakeFiles/STE.dir/main.cpp.o" \
"CMakeFiles/STE.dir/Console.cpp.o" \
"CMakeFiles/STE.dir/MyString.cpp.o"

# External object files for target STE
STE_EXTERNAL_OBJECTS =

STE: CMakeFiles/STE.dir/main.cpp.o
STE: CMakeFiles/STE.dir/Console.cpp.o
STE: CMakeFiles/STE.dir/MyString.cpp.o
STE: CMakeFiles/STE.dir/build.make
STE: CMakeFiles/STE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable STE"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/STE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/STE.dir/build: STE

.PHONY : CMakeFiles/STE.dir/build

CMakeFiles/STE.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/STE.dir/cmake_clean.cmake
.PHONY : CMakeFiles/STE.dir/clean

CMakeFiles/STE.dir/depend:
	cd "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE" "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE" "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug" "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug" "/home/maxim/IBKS - 3 course/OOP/Labs/Lab 03/STE/cmake-build-debug/CMakeFiles/STE.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/STE.dir/depend

