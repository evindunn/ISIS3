#!/bin/bash -l
set -e

# Checkout
export STAGE_STATUS="Checkout on $STAGE_NAME"
conda create -n isis3 python=3.6
conda activate isis3
conda config --env --add channels conda-forge
conda config --env --add channels usgs-astrogeology

conda env update -n isis3 -f $ISIS_ENV_FILE

mkdir build install
export ISISROOT="$(pwd)/build"
cd build

# Build
export STAGE_STATUS="Build on $STAGE_NAME"
cmake $(CMAKE_FLAGS) ../isis
ninja -j8 install

# Unit tests
export STAGE_STATUS="Unit tests on $STAGE_NAME"
ctest -R _unit_ -j8 -VV

# App tests
export STAGE_STATUS="App tests on $STAGE_NAME"
ctest -R _app_ -j8 -VV

# GTests
export STAGE_STATUS="GTests on $STAGE_NAME"
ctest -R "." -E "(_app_|_unit_|_module_)" -j8 -VV

