#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -e
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export PATH=~/.local/bin:/usr/local/bin:$PATH

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/boostdep
mkdir -p libs/variant2
cp -r $TRAVIS_BUILD_DIR/* libs/variant2
python tools/boostdep/depinst/depinst.py variant2
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

echo "using $TOOLSET : : $COMPILER ;" > ~/user-config.jam
./b2 libs/variant2/test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release ${UBSAN:+cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=undefined linkflags=-fsanitize=undefined define=UBSAN=1 debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}

elif [ "$DRONE_JOB_BUILDTYPE" == "bdf8b0b5ef-e906480451" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/boostdep
mkdir -p libs/variant2
cp -r $TRAVIS_BUILD_DIR/* libs/variant2
python tools/boostdep/depinst/depinst.py variant2
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

mkdir __build__ && cd __build__
cmake -DBOOST_ENABLE_CMAKE=1 -DBoost_VERBOSE=1 -DBOOST_INCLUDE_LIBRARIES=variant2 ..
ctest --output-on-failure -R boost_variant2

elif [ "$DRONE_JOB_BUILDTYPE" == "7e8aa85da9-fda7c76df5" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
git clone -b $BOOST_BRANCH https://github.com/boostorg/assert.git ../assert
git clone -b $BOOST_BRANCH https://github.com/boostorg/config.git ../config
git clone -b $BOOST_BRANCH https://github.com/boostorg/core.git ../core
git clone -b $BOOST_BRANCH https://github.com/boostorg/mp11.git ../mp11

echo '==================================> SCRIPT'

cd test/cmake_subdir_test && mkdir __build__ && cd __build__
cmake ..
cmake --build .
cmake --build . --target check

elif [ "$DRONE_JOB_BUILDTYPE" == "bdf8b0b5ef-863debdd74" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/boostdep
mkdir -p libs/variant2
cp -r $TRAVIS_BUILD_DIR/* libs/variant2
python tools/boostdep/depinst/depinst.py variant2
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

pip install --user cmake
mkdir __build__ && cd __build__
cmake -DBOOST_ENABLE_CMAKE=1 -DBoost_VERBOSE=1 -DBOOST_INCLUDE_LIBRARIES=variant2 -DCMAKE_INSTALL_PREFIX=~/.local ..
cmake --build . --target install
cd ../libs/variant2/test/cmake_install_test && mkdir __build__ && cd __build__
cmake -DCMAKE_INSTALL_PREFIX=~/.local ..
cmake --build .
cmake --build . --target check

fi
