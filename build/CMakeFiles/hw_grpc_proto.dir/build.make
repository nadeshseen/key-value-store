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
CMAKE_COMMAND = /home/nadesh/.local/bin/cmake

# The command to remove a file.
RM = /home/nadesh/.local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build

# Include any dependencies generated for this target.
include CMakeFiles/hw_grpc_proto.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw_grpc_proto.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw_grpc_proto.dir/flags.make

KV_Store.pb.cc: ../KV_Store.proto
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating KV_Store.pb.cc, KV_Store.pb.h, KV_Store.grpc.pb.cc, KV_Store.grpc.pb.h"
	/home/nadesh/.local/bin/protoc-3.15.8.0 --grpc_out /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build --cpp_out /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build -I /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store --plugin=protoc-gen-grpc="/home/nadesh/.local/bin/grpc_cpp_plugin" /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/KV_Store.proto

KV_Store.pb.h: KV_Store.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate KV_Store.pb.h

KV_Store.grpc.pb.cc: KV_Store.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate KV_Store.grpc.pb.cc

KV_Store.grpc.pb.h: KV_Store.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate KV_Store.grpc.pb.h

CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.o: CMakeFiles/hw_grpc_proto.dir/flags.make
CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.o: KV_Store.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.o -c /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/KV_Store.grpc.pb.cc

CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/KV_Store.grpc.pb.cc > CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.i

CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/KV_Store.grpc.pb.cc -o CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.s

CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.o: CMakeFiles/hw_grpc_proto.dir/flags.make
CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.o: KV_Store.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.o -c /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/KV_Store.pb.cc

CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/KV_Store.pb.cc > CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.i

CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/KV_Store.pb.cc -o CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.s

# Object files for target hw_grpc_proto
hw_grpc_proto_OBJECTS = \
"CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.o" \
"CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.o"

# External object files for target hw_grpc_proto
hw_grpc_proto_EXTERNAL_OBJECTS =

libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/KV_Store.grpc.pb.cc.o
libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/KV_Store.pb.cc.o
libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/build.make
libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libhw_grpc_proto.a"
	$(CMAKE_COMMAND) -P CMakeFiles/hw_grpc_proto.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw_grpc_proto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw_grpc_proto.dir/build: libhw_grpc_proto.a

.PHONY : CMakeFiles/hw_grpc_proto.dir/build

CMakeFiles/hw_grpc_proto.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw_grpc_proto.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw_grpc_proto.dir/clean

CMakeFiles/hw_grpc_proto.dir/depend: KV_Store.grpc.pb.cc
CMakeFiles/hw_grpc_proto.dir/depend: KV_Store.grpc.pb.h
CMakeFiles/hw_grpc_proto.dir/depend: KV_Store.pb.cc
CMakeFiles/hw_grpc_proto.dir/depend: KV_Store.pb.h
	cd /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles/hw_grpc_proto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw_grpc_proto.dir/depend
