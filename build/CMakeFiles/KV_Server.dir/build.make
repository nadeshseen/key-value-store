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
include CMakeFiles/KV_Server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/KV_Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/KV_Server.dir/flags.make

CMakeFiles/KV_Server.dir/KV_Server.cc.o: CMakeFiles/KV_Server.dir/flags.make
CMakeFiles/KV_Server.dir/KV_Server.cc.o: ../KV_Server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/KV_Server.dir/KV_Server.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/KV_Server.dir/KV_Server.cc.o -c /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/KV_Server.cc

CMakeFiles/KV_Server.dir/KV_Server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KV_Server.dir/KV_Server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/KV_Server.cc > CMakeFiles/KV_Server.dir/KV_Server.cc.i

CMakeFiles/KV_Server.dir/KV_Server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KV_Server.dir/KV_Server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/KV_Server.cc -o CMakeFiles/KV_Server.dir/KV_Server.cc.s

# Object files for target KV_Server
KV_Server_OBJECTS = \
"CMakeFiles/KV_Server.dir/KV_Server.cc.o"

# External object files for target KV_Server
KV_Server_EXTERNAL_OBJECTS =

KV_Server: CMakeFiles/KV_Server.dir/KV_Server.cc.o
KV_Server: CMakeFiles/KV_Server.dir/build.make
KV_Server: libhw_grpc_proto.a
KV_Server: /home/nadesh/.local/lib/libgrpc++_reflection.a
KV_Server: /home/nadesh/.local/lib/libgrpc++.a
KV_Server: /home/nadesh/.local/lib/libprotobuf.a
KV_Server: /home/nadesh/.local/lib/libgrpc.a
KV_Server: /home/nadesh/.local/lib/libz.a
KV_Server: /home/nadesh/.local/lib/libcares.a
KV_Server: /home/nadesh/.local/lib/libaddress_sorting.a
KV_Server: /home/nadesh/.local/lib/libre2.a
KV_Server: /home/nadesh/.local/lib/libabsl_hash.a
KV_Server: /home/nadesh/.local/lib/libabsl_city.a
KV_Server: /home/nadesh/.local/lib/libabsl_wyhash.a
KV_Server: /home/nadesh/.local/lib/libabsl_raw_hash_set.a
KV_Server: /home/nadesh/.local/lib/libabsl_hashtablez_sampler.a
KV_Server: /home/nadesh/.local/lib/libabsl_exponential_biased.a
KV_Server: /home/nadesh/.local/lib/libabsl_statusor.a
KV_Server: /home/nadesh/.local/lib/libabsl_bad_variant_access.a
KV_Server: /home/nadesh/.local/lib/libgpr.a
KV_Server: /home/nadesh/.local/lib/libupb.a
KV_Server: /home/nadesh/.local/lib/libabsl_status.a
KV_Server: /home/nadesh/.local/lib/libabsl_cord.a
KV_Server: /home/nadesh/.local/lib/libabsl_str_format_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_synchronization.a
KV_Server: /home/nadesh/.local/lib/libabsl_stacktrace.a
KV_Server: /home/nadesh/.local/lib/libabsl_symbolize.a
KV_Server: /home/nadesh/.local/lib/libabsl_debugging_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_demangle_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_graphcycles_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_malloc_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_time.a
KV_Server: /home/nadesh/.local/lib/libabsl_strings.a
KV_Server: /home/nadesh/.local/lib/libabsl_throw_delegate.a
KV_Server: /home/nadesh/.local/lib/libabsl_strings_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_base.a
KV_Server: /home/nadesh/.local/lib/libabsl_spinlock_wait.a
KV_Server: /home/nadesh/.local/lib/libabsl_int128.a
KV_Server: /home/nadesh/.local/lib/libabsl_civil_time.a
KV_Server: /home/nadesh/.local/lib/libabsl_time_zone.a
KV_Server: /home/nadesh/.local/lib/libabsl_bad_optional_access.a
KV_Server: /home/nadesh/.local/lib/libabsl_raw_logging_internal.a
KV_Server: /home/nadesh/.local/lib/libabsl_log_severity.a
KV_Server: /home/nadesh/.local/lib/libssl.a
KV_Server: /home/nadesh/.local/lib/libcrypto.a
KV_Server: CMakeFiles/KV_Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable KV_Server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/KV_Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/KV_Server.dir/build: KV_Server

.PHONY : CMakeFiles/KV_Server.dir/build

CMakeFiles/KV_Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/KV_Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/KV_Server.dir/clean

CMakeFiles/KV_Server.dir/depend:
	cd /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build /media/nadesh/Extra/IITB/CS744/Assignment/5/key-value-store/build/CMakeFiles/KV_Server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/KV_Server.dir/depend

