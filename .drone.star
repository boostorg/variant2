# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2020.

# For Drone CI we use the Starlark scripting language to reduce duplication.
# As the yaml syntax for Drone CI is rather limited.
#
#
globalenv={}
linuxglobalimage="cppalliance/droneubuntu1604:1"
windowsglobalimage="cppalliance/dronevs2019"

def main(ctx):
  return [
  linux_cxx("TOOLSET=gcc COMPILER=g++-4.8 CXXSTD=11 Job 0", "g++-4.8", packages="g++-4.8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.8', 'CXXSTD': '11', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-4.9 CXXSTD=11 Job 1", "g++-4.9", packages="g++-4.9", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.9', 'CXXSTD': '11', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-5 CXXSTD=11,14,1z Job 2", "g++-5", packages="g++-5", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-5', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-6 CXXSTD=11,14,1z Job 3", "g++-6", packages="g++-6", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-6', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-7 CXXSTD=11,14,17 Job 4", "g++-7", packages="g++-7", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-7', 'CXXSTD': '11,14,17', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-8 CXXSTD=11,14,17,2a Job 5", "g++-8", packages="g++-8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-8', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-9 CXXSTD=11,14,17,2a Job 6", "g++-9", packages="g++-9", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-9', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-10 CXXSTD=11,14,17,2 Job 7", "g++-10", packages="g++-10", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-10', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  linux_cxx("UBSAN=1 TOOLSET=gcc COMPILER=g++-8 CXXSTD=11, Job 8", "g++-8", packages="g++-8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'UBSAN': '1', 'TOOLSET': 'gcc', 'COMPILER': 'g++-8', 'CXXSTD': '11,14,17,2a', 'UBSAN_OPTIONS': 'print_stacktrace=1', 'LINKFLAGS': '-fuse-ld=gold', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("DIST=trusty TOOLSET=clang COMPILER=clang++ CX Job 9", "clang++", packages="", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'DIST': 'trusty', 'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-3.5 CXXSTD=11, Job 10", "clang++-3.5", packages="clang-3.5 libstdc++-4.9-dev", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.5', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-3.6 CXXSTD=11, Job 11", "clang++-3.6", packages="clang-3.6", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.6', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-3.7 CXXSTD=11, Job 12", "clang++-3.7", packages="clang-3.7", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.7', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': '7b52009b64'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-3.8 CXXSTD=11, Job 13", "clang++-3.8", packages="clang-3.8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.8', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'bd307a3ec3'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-3.9 CXXSTD=11, Job 14", "clang++-3.9", packages="clang-3.9", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.9', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'fa35e19212'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-4.0 CXXSTD=11, Job 15", "clang++-4.0", packages="clang-4.0", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-4.0', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'f1abd67035'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-5.0 CXXSTD=11, Job 16", "clang++-5.0", packages="clang-5.0", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-5.0', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': '1574bddb75'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-6.0 CXXSTD=11, Job 17", "clang++-6.0", packages="clang-6.0", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-6.0', 'CXXSTD': '11,14,17', 'DRONE_JOB_UUID': '0716d9708d'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-7 CXXSTD=11,14 Job 18", "clang++-7", packages="clang-7", llvm_os="xenial", llvm_ver="7", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-7', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': '9e6a55b6b4'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-8 CXXSTD=11,14 Job 19", "clang++-8", packages="clang-8", llvm_os="xenial", llvm_ver="8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-8', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': 'b3f0c7f6bb'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-9 CXXSTD=11,14 Job 20", "clang++-9", packages="clang-9", llvm_os="xenial", llvm_ver="9", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-9', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': '91032ad7bb'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-10 CXXSTD=11,1 Job 21", "clang++-10", packages="clang-10", llvm_os="xenial", llvm_ver="10", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-10', 'CXXSTD': '11,14,17,2a', 'DRONE_JOB_UUID': '472b07b9fc'}, globalenv=globalenv),
  linux_cxx("UBSAN=1 TOOLSET=clang COMPILER=clang++-8 CXXS Job 22", "clang++-8", packages="clang-8", llvm_os="xenial", llvm_ver="8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'UBSAN': '1', 'TOOLSET': 'clang', 'COMPILER': 'clang++-8', 'CXXSTD': '11,14,17,2a', 'UBSAN_OPTIONS': 'print_stacktrace=1', 'DRONE_JOB_UUID': '12c6fc06c9'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-libc++ CXXSTD= Job 23", "clang++-libc++", packages="libc++-dev", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-libc++', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'd435a6cdd7'}, globalenv=globalenv),
  linux_cxx("UBSAN=1 TOOLSET=clang COMPILER=clang++-libc++ Job 24", "clang++-libc++", packages="libc++-dev", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'UBSAN': '1', 'TOOLSET': 'clang', 'COMPILER': 'clang++-libc++', 'CXXSTD': '11,14,1z', 'UBSAN_OPTIONS': 'print_stacktrace=1', 'DRONE_JOB_UUID': '4d134bc072'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang COMPILER=clang++ CXXSTD=11,14,1 Job 25", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '11,14,1z', 'DRONE_JOB_UUID': 'f6e1126ced'}, globalenv=globalenv),
  osx_cxx("UBSAN=1 TOOLSET=clang COMPILER=clang++ CXXSTD Job 26", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'UBSAN': '1', 'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '11,14,1z', 'UBSAN_OPTIONS': 'print_stacktrace=1', 'DRONE_JOB_UUID': '887309d048'}, globalenv=globalenv),
  linux_cxx("CMAKE_TEST=1 Job 27", "g++", packages="", buildtype="bdf8b0b5ef-e906480451", buildscript="drone", image=linuxglobalimage, environment={'CMAKE_TEST': '1', 'DRONE_JOB_UUID': 'bc33ea4e26'}, globalenv=globalenv),
  linux_cxx("CMAKE_SUBDIR_TEST=1 Job 28", "g++", packages="", buildtype="7e8aa85da9-fda7c76df5", buildscript="drone", image=linuxglobalimage, environment={'CMAKE_SUBDIR_TEST': '1', 'DRONE_JOB_UUID': '0a57cb53ba'}, globalenv=globalenv),
  linux_cxx("CMAKE_INSTALL_TEST=1 Job 29", "g++", packages="", buildtype="bdf8b0b5ef-863debdd74", buildscript="drone", image=linuxglobalimage, environment={'CMAKE_INSTALL_TEST': '1', 'DRONE_JOB_UUID': '7719a1c782'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
