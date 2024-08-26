# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindLibXar
--------------

Find libxar library and headers.

The module defines the following variables:

::

  LibXar_FOUND        - true if libxar was found
  LibXar_INCLUDE_DIRS - include search path
  LibXar_LIBRARIES    - libraries to link
  LibXar_VERSION      - libxar 3-component version number

The module defines the following ``IMPORTED`` targets:

::

  LibXar::LibXar  - target for linking against libxar

#]=======================================================================]

cmake_policy(PUSH)
cmake_policy(SET CMP0159 NEW) # file(STRINGS) with REGEX updates CMAKE_MATCH_<n>

unset(LibXar_FOUND)

find_path(LibXar_INCLUDE_DIR
  NAMES xar.h
  PATHS "/usr/include/xar" "/usr/local/include/xar"
  DOC "libxar include directory"
  )

find_library(LibXar_LIBRARY
  NAMES libxar.so
  PATHS "/usr/lib" "/usr/local/lib" "/lib"
  DOC "libxar library"
  )

mark_as_advanced(LibXar_INCLUDE_DIR LibXar_LIBRARY)

# Extract the version number from the header.
if(LibXar_INCLUDE_DIR AND EXISTS "${LibXar_INCLUDE_DIR}/xar.h")
  # The version string appears in one of three known formats in the header:
  #  #define XAR_VERSION "1.5.2"
  # Match any format.
  set(_LibXar_VERSION_REGEX "#define XAR_VERSION \"([^\"]+)\"")
  file(STRINGS "${LibXar_INCLUDE_DIR}/xar.h" _LibXar_VERSION_STRING LIMIT_COUNT 1 REGEX "${_LibXar_VERSION_REGEX}")
  if(_LibXar_VERSION_STRING)
    string(REGEX REPLACE "${_LibXar_VERSION_REGEX}" "\\1" LibXar_VERSION "${_LibXar_VERSION_STRING}")
  endif()
  unset(_LibXar_VERSION_REGEX)
  unset(_LibXar_VERSION_STRING)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibXar
                                  REQUIRED_VARS LibXar_LIBRARY LibXar_INCLUDE_DIR
                                  VERSION_VAR LibXar_VERSION
  )

if(LibXar_FOUND)
  set(LibXar_INCLUDE_DIRS ${LibXar_INCLUDE_DIR})
  set(LibXar_LIBRARIES    ${LibXar_LIBRARY})

  if (NOT TARGET LibXar::LibXar)
    add_library(LibXar::LibXar UNKNOWN IMPORTED)
    set_target_properties(LibXar::LibXar PROPERTIES
      IMPORTED_LOCATION "${LibXar_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${LibXar_INCLUDE_DIR}")
  endif ()
endif()

cmake_policy(POP)
