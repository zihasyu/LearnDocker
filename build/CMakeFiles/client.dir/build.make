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
CMAKE_SOURCE_DIR = /home/zihasyu/WZY/LearnDocker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zihasyu/WZY/LearnDocker/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/src/app/client_main.cc.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/app/client_main.cc.o: ../src/app/client_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zihasyu/WZY/LearnDocker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/src/app/client_main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/src/app/client_main.cc.o -c /home/zihasyu/WZY/LearnDocker/src/app/client_main.cc

CMakeFiles/client.dir/src/app/client_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/app/client_main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zihasyu/WZY/LearnDocker/src/app/client_main.cc > CMakeFiles/client.dir/src/app/client_main.cc.i

CMakeFiles/client.dir/src/app/client_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/app/client_main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zihasyu/WZY/LearnDocker/src/app/client_main.cc -o CMakeFiles/client.dir/src/app/client_main.cc.s

CMakeFiles/client.dir/src/proto/cache.pb.cc.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/proto/cache.pb.cc.o: ../src/proto/cache.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zihasyu/WZY/LearnDocker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/src/proto/cache.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/src/proto/cache.pb.cc.o -c /home/zihasyu/WZY/LearnDocker/src/proto/cache.pb.cc

CMakeFiles/client.dir/src/proto/cache.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/proto/cache.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zihasyu/WZY/LearnDocker/src/proto/cache.pb.cc > CMakeFiles/client.dir/src/proto/cache.pb.cc.i

CMakeFiles/client.dir/src/proto/cache.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/proto/cache.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zihasyu/WZY/LearnDocker/src/proto/cache.pb.cc -o CMakeFiles/client.dir/src/proto/cache.pb.cc.s

CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.o: ../src/proto/cache.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zihasyu/WZY/LearnDocker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.o -c /home/zihasyu/WZY/LearnDocker/src/proto/cache.grpc.pb.cc

CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zihasyu/WZY/LearnDocker/src/proto/cache.grpc.pb.cc > CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.i

CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zihasyu/WZY/LearnDocker/src/proto/cache.grpc.pb.cc -o CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/src/app/client_main.cc.o" \
"CMakeFiles/client.dir/src/proto/cache.pb.cc.o" \
"CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/src/app/client_main.cc.o
client: CMakeFiles/client.dir/src/proto/cache.pb.cc.o
client: CMakeFiles/client.dir/src/proto/cache.grpc.pb.cc.o
client: CMakeFiles/client.dir/build.make
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zihasyu/WZY/LearnDocker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/zihasyu/WZY/LearnDocker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zihasyu/WZY/LearnDocker /home/zihasyu/WZY/LearnDocker /home/zihasyu/WZY/LearnDocker/build /home/zihasyu/WZY/LearnDocker/build /home/zihasyu/WZY/LearnDocker/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

