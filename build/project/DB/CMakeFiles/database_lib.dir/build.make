# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = "/home/maxim/Рабочий стол/DB+SERVER"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/maxim/Рабочий стол/DB+SERVER/build"

# Include any dependencies generated for this target.
include project/DB/CMakeFiles/database_lib.dir/depend.make

# Include the progress variables for this target.
include project/DB/CMakeFiles/database_lib.dir/progress.make

# Include the compile flags for this target's objects.
include project/DB/CMakeFiles/database_lib.dir/flags.make

project/DB/CMakeFiles/database_lib.dir/src/models.cpp.o: project/DB/CMakeFiles/database_lib.dir/flags.make
project/DB/CMakeFiles/database_lib.dir/src/models.cpp.o: ../project/DB/src/models.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/maxim/Рабочий стол/DB+SERVER/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object project/DB/CMakeFiles/database_lib.dir/src/models.cpp.o"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/database_lib.dir/src/models.cpp.o -c "/home/maxim/Рабочий стол/DB+SERVER/project/DB/src/models.cpp"

project/DB/CMakeFiles/database_lib.dir/src/models.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/database_lib.dir/src/models.cpp.i"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/maxim/Рабочий стол/DB+SERVER/project/DB/src/models.cpp" > CMakeFiles/database_lib.dir/src/models.cpp.i

project/DB/CMakeFiles/database_lib.dir/src/models.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/database_lib.dir/src/models.cpp.s"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/maxim/Рабочий стол/DB+SERVER/project/DB/src/models.cpp" -o CMakeFiles/database_lib.dir/src/models.cpp.s

project/DB/CMakeFiles/database_lib.dir/src/postgresql_db.cpp.o: project/DB/CMakeFiles/database_lib.dir/flags.make
project/DB/CMakeFiles/database_lib.dir/src/postgresql_db.cpp.o: ../project/DB/src/postgresql_db.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/maxim/Рабочий стол/DB+SERVER/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object project/DB/CMakeFiles/database_lib.dir/src/postgresql_db.cpp.o"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/database_lib.dir/src/postgresql_db.cpp.o -c "/home/maxim/Рабочий стол/DB+SERVER/project/DB/src/postgresql_db.cpp"

project/DB/CMakeFiles/database_lib.dir/src/postgresql_db.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/database_lib.dir/src/postgresql_db.cpp.i"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/maxim/Рабочий стол/DB+SERVER/project/DB/src/postgresql_db.cpp" > CMakeFiles/database_lib.dir/src/postgresql_db.cpp.i

project/DB/CMakeFiles/database_lib.dir/src/postgresql_db.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/database_lib.dir/src/postgresql_db.cpp.s"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/maxim/Рабочий стол/DB+SERVER/project/DB/src/postgresql_db.cpp" -o CMakeFiles/database_lib.dir/src/postgresql_db.cpp.s

# Object files for target database_lib
database_lib_OBJECTS = \
"CMakeFiles/database_lib.dir/src/models.cpp.o" \
"CMakeFiles/database_lib.dir/src/postgresql_db.cpp.o"

# External object files for target database_lib
database_lib_EXTERNAL_OBJECTS =

project/DB/libdatabase_lib.so: project/DB/CMakeFiles/database_lib.dir/src/models.cpp.o
project/DB/libdatabase_lib.so: project/DB/CMakeFiles/database_lib.dir/src/postgresql_db.cpp.o
project/DB/libdatabase_lib.so: project/DB/CMakeFiles/database_lib.dir/build.make
project/DB/libdatabase_lib.so: project/DB/CMakeFiles/database_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/maxim/Рабочий стол/DB+SERVER/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libdatabase_lib.so"
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/database_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
project/DB/CMakeFiles/database_lib.dir/build: project/DB/libdatabase_lib.so

.PHONY : project/DB/CMakeFiles/database_lib.dir/build

project/DB/CMakeFiles/database_lib.dir/clean:
	cd "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" && $(CMAKE_COMMAND) -P CMakeFiles/database_lib.dir/cmake_clean.cmake
.PHONY : project/DB/CMakeFiles/database_lib.dir/clean

project/DB/CMakeFiles/database_lib.dir/depend:
	cd "/home/maxim/Рабочий стол/DB+SERVER/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/maxim/Рабочий стол/DB+SERVER" "/home/maxim/Рабочий стол/DB+SERVER/project/DB" "/home/maxim/Рабочий стол/DB+SERVER/build" "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB" "/home/maxim/Рабочий стол/DB+SERVER/build/project/DB/CMakeFiles/database_lib.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : project/DB/CMakeFiles/database_lib.dir/depend

