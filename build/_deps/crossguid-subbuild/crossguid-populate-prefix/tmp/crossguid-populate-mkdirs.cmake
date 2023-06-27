# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-src"
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-build"
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix"
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix/tmp"
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix/src/crossguid-populate-stamp"
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix/src"
  "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix/src/crossguid-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix/src/crossguid-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Adrian/Desktop/Velce/build/_deps/crossguid-subbuild/crossguid-populate-prefix/src/crossguid-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
