# ------------------------------------------------
# Makefile to run unit tests and create a driver
# for a lidar data processing program
#
# Original Author: olschanowsky
# Modified by: ravishankar
# Date  : 18 May 2017
# ------------------------------------------------

# Usage
#######
# make test        - creates and runs all the unit tests
# make geotiff-driver - creates the main executable in bin/
# make fitting info - creates a gaussian fitting checking tool
# make pls-info    - creates a .pls file info checking tool
# make clean       - removes all files generated by make.


# Please tweak the following variable definitions as needed by your
# project, except GTEST_HEADERS, which you can use in your own targets
# but shouldn't modify.

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = deps/googletest/googletest

# Points to the root of Pulse Waves, relative to where this file is.
PULSE_DIR = deps/PulseWaves

# Where to find user code, relative to where this file is.
SRC = src

# Different directories, relative to where this file is.
BIN = bin
OBJ = obj
LIB = lib

# Flags passed to the preprocessor
##################################
# Set Google Test's header directory as a system directory, so that
# the compiler doesn't generate warnings in Google Test headers.
#
# -isystem dir: Mark the directory 'dir'as a system directory, so that it
#               gets the same special treatment that is applied to the 
#               standard system directories
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C and C++ compiler
########################################
# -std=c++11: Set standard to C++11
#
# -g:         Produce debugging information in the operating system’s 
#             native format
#
# -Wall:      Enables all warning flags 
#             (https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
#
# -Wextra:    This enables extra warning flags that are not enabled by -Wall
#
# -pthread:   Link with the POSIX threads library
#
# -Idir:      Add the directory 'dir' to the list of directories to be searched 
#             for heade# files during preprocessing
#
# -llib:      Search the library named 'lib' when linking
CXXFLAGS += -std=c++11 -g -Wall -Wextra -pthread -I$(PULSE_DIR)/inc \
			-Ideps
CFLAGS += -std=c++11 -g -Wall -Wextra -pthread -I$(PULSE_DIR)/inc \
		  -Ideps

# If this is a profiler build, add -pg flag to all uses of CXX
ifdef PROFILER_BUILD
PFLAG += -pg
else
PFLAG =
endif

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = $(BIN)/CmdLine_unittests $(BIN)/FlightLineData_unittests \
		$(BIN)/WaveGPSInformation_unittests $(BIN)/PulseData_unittests \
		$(BIN)/LidarVolume_unittests $(BIN)/GaussianFitter_unittests \
		$(BIN)/LidarDriver_unittests $(BIN)/Peak_unittests \
		$(BIN)/csv_CmdLine_unittests $(BIN)/TxtWaveReader_unittests

# All Google Test headers.  Usually you shouldn't change this definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Builds gtest.a and gtest_main.a.
# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(OBJ)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(OBJ)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(LIB)/gtest.a : $(OBJ)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(LIB)/gtest_main.a : $(OBJ)/gtest-all.o $(OBJ)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a/all test/s.  
# A test should link with either gtest.a or gtest_main.a, 
# depending on whether it defines its own main() function.

$(BIN)/GaussianFitter_unittests: $(OBJ)/GaussianFitter.o \
                                 $(OBJ)/GaussianFitter_unittests.o \
				 $(LIB)/gtest_main.a $(OBJ)/Peak.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L\
		$(PULSE_DIR)/lib -lpulsewaves -lm -lgsl -lgslcblas -lgdal

$(BIN)/FlightLineData_unittests: $(OBJ)/FlightLineData_unittests.o \
                                 $(OBJ)/FlightLineData.o $(OBJ)/PulseData.o \
                                 $(OBJ)/WaveGPSInformation.o \
                                 $(LIB)/gtest_main.a \
                                 $(OBJ)/WaveGPSInformation.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves

$(BIN)/LidarVolume_unittests: $(OBJ)/LidarVolume_unittests.o \
                              $(OBJ)/LidarVolume.o $(OBJ)/FlightLineData.o \
                              $(OBJ)/Peak.o \
                              $(OBJ)/WaveGPSInformation.o $(LIB)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves -lgdal

$(BIN)/LidarDriver_unittests: $(OBJ)/LidarDriver_unittests.o \
                              $(OBJ)/CmdLine.o \
                              $(OBJ)/FlightLineData.o $(OBJ)/LidarVolume.o \
                              $(OBJ)/LidarDriver.o $(OBJ)/WaveGPSInformation.o\
                              $(OBJ)/PulseData.o $(OBJ)/TxtWaveReader.o\
                              $(OBJ)/Peak.o $(OBJ)/GaussianFitter.o\
                              $(LIB)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves -lgdal -lgsl -lgslcblas

$(BIN)/TxtWaveReader_unittests: $(OBJ)/TxtWaveReader_unittests.o \
                                $(OBJ)/TxtWaveReader.o \
                                $(OBJ)/PulseData.o $(LIB)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves

$(BIN)/%_unittests: $(OBJ)/%_unittests.o $(OBJ)/%.o $(LIB)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves

# Builds specific object files
$(OBJ)/GaussianFitter_unittests.o: $(SRC)/GaussianFitter_unittests.cpp
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -c \
		-o $@ $^

$(OBJ)/%_unittests.o: $(SRC)/%_unittests.cpp
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

$(OBJ)/FlightLineData.o: $(SRC)/FlightLineData.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -L$(PULSE_DIR)/lib

$(OBJ)//WaveGPSInformation.o: $(SRC)/WaveGPSInformation.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -L$(PULSE_DIR)/lib

$(OBJ)/PulseData.o: $(SRC)/PulseData.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -L$(PULSE_DIR)/lib

$(OBJ)//LidarVolume.o: $(SRC)/LidarVolume.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -lgdal -L$(PULSE_DIR)/lib

$(OBJ)/GaussianFitter.o: $(SRC)/GaussianFitter.cpp
	$(CXX) $(PFLAG) -Ideps -std=c++11 -g -fpermissive -c -o $@ $^ -lm \
		-lgsl -lgslcblas

$(OBJ)/LidarDriver.o: $(SRC)/LidarDriver.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -L$(PULSE_DIR)/lib

$(OBJ)/TxtWaveReader.o: $(SRC)/TxtWaveReader.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -L$(PULSE_DIR)/lib

# Builds all object files
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS)

# Builds the info tool
pls-info: $(BIN)/pls-info

$(BIN)/pls-info: $(OBJ)/GetPLSDetails.o $(OBJ)/PulseData.o
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -g -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves

$(OBJ)/GetPLSDetails.o: $(SRC)/GetPLSDetails.cpp
	$(CXX) $(PFLAG) -c -o $@ $^ $(CFLAGS) -L$(PULSE_DIR)/lib

# Builds the main driver file 
geotiff-driver: $(BIN)/geotiff-driver

$(BIN)/geotiff-driver: $(OBJ)/pls_to_geotiff.o $(OBJ)/CmdLine.o \
                       $(OBJ)/FlightLineData.o $(OBJ)/LidarVolume.o \
                       $(OBJ)/LidarDriver.o $(OBJ)/WaveGPSInformation.o\
                       $(OBJ)/WaveGPSInformation.o $(OBJ)/PulseData.o \
                       $(OBJ)/Peak.o $(OBJ)/GaussianFitter.o
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -g -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves -lgdal -lm -lgsl \
		-lgslcblas

$(OBJ)/pls_to_geotiff.o: $(SRC)/pls_to_geotiff.cpp
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

# Builds the csv driver file

csv-driver: $(BIN)/csv-driver

$(BIN)/csv-driver: $(OBJ)/pls_to_csv.o $(OBJ)/csv_CmdLine.o \
                   $(OBJ)/FlightLineData.o $(OBJ)/LidarVolume.o \
				   $(OBJ)/LidarDriver.o $(OBJ)/WaveGPSInformation.o \
				   $(OBJ)/PulseData.o $(OBJ)/Peak.o $(OBJ)/GaussianFitter.o \
				   $(OBJ)/CsvWriter.o $(OBJ)/TxtWaveReader.o
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -g -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves -lgdal -lm -lgsl \
		-lgslcblas

# Builds the fitting information tool
fitting-info: $(BIN)/fitting-info

$(BIN)/fitting-info: $(OBJ)/fitting_info.o $(OBJ)/FlightLineData.o \
                     $(OBJ)/PulseData.o $(OBJ)/Peak.o \
                     $(OBJ)/GaussianFitter.o $(OBJ)/WaveGPSInformation.o \
                     $(OBJ)/FittingInfoDriver.o
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -g -lpthread $^ -o $@ -L \
		$(PULSE_DIR)/lib -lpulsewaves -lgdal -lm -lgsl \
		-lgslcblas

$(OBJ)/fitting_info.o: $(SRC)/fitting_info.cpp
	$(CXX) $(PFLAG) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

# A phony target is one that is not really the name of a file; rather it 
# is just a name for a recipe to be executed when you make an explicit request. 
# There are two reasons to use a phony target: to avoid a conflict with a file 
# of the same name, and to improve performance.
.PHONY: test
.PHONY: clean

# Runs all tests
test: $(TESTS)
	clear
	-$(BIN)/CmdLine_unittests
	-$(BIN)/WaveGPSInformation_unittests
	-$(BIN)/LidarVolume_unittests
	-$(BIN)/PulseData_unittests
	-$(BIN)/GaussianFitter_unittests
	-$(BIN)/LidarDriver_unittests
	-$(BIN)/FlightLineData_unittests
	-$(BIN)/Peak_unittests
	-$(BIN)/csv_CmdLine_unittests
	-$(BIN)/TxtWaveReader_unittests

# Clean up when done. 
# Removes all object, library and executable files
clean:
	clear
	rm -f $(BIN)/*
	rm -f $(OBJ)/*
	rm -f $(LIB)/*
	rm -f $(SRC)/*.o
