#!/bin/bash -l
set -e

conda create -n isis3 python=3.6
conda activate isis3
conda config --env --add channels conda-forge
conda config --env --add channels usgs-astrogeology

git clone --recurse-submodules https://github.com/USGS-Astrogeology/ISIS3.git
cd ISIS3
conda env update -n isis3 -f environment_gcc4.yml

mkdir build install
cd build

cmake -Disis3Data=/isisData/data -Disis3TestData=/isisData/testData -DJP2KFLAG=OFF -DCMAKE_BUILD_TYPE=RELEASE -GNinja ../isis

export ISISROOT=$(pwd)

ninja -j4 install

