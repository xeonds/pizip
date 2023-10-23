#!/bin/bash

# Define paths
PROJECT_ROOT=$(pwd)
CXXOPTS_SUBMODULE_PATH=external/cxxopts

# Update the submodule
echo "Updating cxxopts submodule..."
git submodule update --init --recursive

# Build the cxxopts library
echo "Building cxxopts..."
cd $CXXOPTS_SUBMODULE_PATH
mkdir -p build && cd build
cmake ..
make

# Return to the project root
cd $PROJECT_ROOT

# Build the main program
echo "Building the main program..."
mkdir -p build && cd build
cmake ..
make

# Copy the final executable to the project root
cp pizip $PROJECT_ROOT

echo "Build completed. The executable 'FileCompressor' is available in the project root."

