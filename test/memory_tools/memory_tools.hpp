// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Code to do replacing of malloc/free/etc... inspired by:
//   https://dxr.mozilla.org/mozilla-central/rev/
//    cc9c6cd756cb744596ba039dcc5ad3065a7cc3ea/memory/build/replace_malloc.c

#ifndef MEMORY_TOOLS__MEMORY_TOOLS_HPP_
#define MEMORY_TOOLS__MEMORY_TOOLS_HPP_

#include <stddef.h>

#include <functional>

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define RCUTILS_MEMORY_TOOLS_EXPORT __attribute__ ((dllexport))
    #define RCUTILS_MEMORY_TOOLS_IMPORT __attribute__ ((dllimport))
  #else
    #define RCUTILS_MEMORY_TOOLS_EXPORT __declspec(dllexport)
    #define RCUTILS_MEMORY_TOOLS_IMPORT __declspec(dllimport)
  #endif
  #ifdef RCUTILS_MEMORY_TOOLS_BUILDING_DLL
    #define RCUTILS_MEMORY_TOOLS_PUBLIC RCUTILS_MEMORY_TOOLS_EXPORT
  #else
    #define RCUTILS_MEMORY_TOOLS_PUBLIC RCUTILS_MEMORY_TOOLS_IMPORT
  #endif
  #define RCUTILS_MEMORY_TOOLS_PUBLIC_TYPE RCUTILS_MEMORY_TOOLS_PUBLIC
  #define RCUTILS_MEMORY_TOOLS_LOCAL
#else
  #define RCUTILS_MEMORY_TOOLS_EXPORT __attribute__ ((visibility("default")))
  #define RCUTILS_MEMORY_TOOLS_IMPORT
  #if __GNUC__ >= 4
    #define RCUTILS_MEMORY_TOOLS_PUBLIC __attribute__ ((visibility("default")))
    #define RCUTILS_MEMORY_TOOLS_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define RCUTILS_MEMORY_TOOLS_PUBLIC
    #define RCUTILS_MEMORY_TOOLS_LOCAL
  #endif
  #define RCUTILS_MEMORY_TOOLS_PUBLIC_TYPE
#endif

typedef std::function<void ()> UnexpectedCallbackType;

RCUTILS_MEMORY_TOOLS_PUBLIC
void
start_memory_checking();

#define ASSERT_NO_MALLOC(statements) \
  assert_no_malloc_begin(); statements; assert_no_malloc_end();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
assert_no_malloc_begin();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
assert_no_malloc_end();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
set_on_unexpected_malloc_callback(UnexpectedCallbackType callback);

#define ASSERT_NO_REALLOC(statements) \
  assert_no_realloc_begin(); statements; assert_no_realloc_end();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
assert_no_realloc_begin();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
assert_no_realloc_end();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
set_on_unexpected_realloc_callback(UnexpectedCallbackType callback);

#define ASSERT_NO_FREE(statements) \
  assert_no_free_begin(); statements; assert_no_free_end();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
assert_no_free_begin();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
assert_no_free_end();
RCUTILS_MEMORY_TOOLS_PUBLIC
void
set_on_unexpected_free_callback(UnexpectedCallbackType callback);

RCUTILS_MEMORY_TOOLS_PUBLIC
void
stop_memory_checking();

RCUTILS_MEMORY_TOOLS_PUBLIC
void
memory_checking_thread_init();

#endif  // MEMORY_TOOLS__MEMORY_TOOLS_HPP_
