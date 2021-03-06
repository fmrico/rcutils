// Copyright 2017 Open Source Robotics Foundation, Inc.
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

#include <string.h>

#include "rcutils/logging_macros.h"

size_t g_log_calls = 0;

struct LogEvent
{
  rcl_log_location_t * location;
  int severity;
  const char * name;
  char * message;
};
struct LogEvent g_last_log_event;

void custom_handler(
  rcl_log_location_t * location,
  int severity, const char * name, const char * format, va_list * args)
{
  g_log_calls += 1;
  g_last_log_event.location = location;
  g_last_log_event.severity = severity;
  g_last_log_event.name = name ? name : "";
  if (g_last_log_event.message) {
    free(g_last_log_event.message);
  }
  const size_t size = 1024;
  g_last_log_event.message = malloc(size);
  vsnprintf(g_last_log_event.message, size, format, *args);
}

int main(int argc, char ** argv)
{
  (void)argc;
  (void)argv;

  g_log_calls = 0;
  if (g_rcl_logging_initialized) {
    return 1;
  }
  rcl_logging_initialize();
  if (!g_rcl_logging_initialized) {
    return 2;
  }

  rcl_logging_output_handler_t previous_output_handler = \
    rcl_logging_get_output_handler();
  rcl_logging_set_output_handler(custom_handler);

  RCUTILS_LOG_INFO("empty message");
  if (g_log_calls != 1u) {
    return 3;
  }
  if (!g_last_log_event.location) {
    return 4;
  }
  if (strcmp(g_last_log_event.location->function_name, "main")) {
    return 5;
  }
  if (g_last_log_event.location->line_number != 64u) {
    return 6;
  }
  if (g_last_log_event.severity != RCUTILS_LOG_SEVERITY_INFO) {
    return 7;
  }
  if (strcmp(g_last_log_event.name, "")) {
    return 8;
  }
  if (strcmp(g_last_log_event.message, "empty message")) {
    return 9;
  }

  RCUTILS_LOG_INFO("message %s", "foo");
  if (g_log_calls != 2u) {
    return 10;
  }
  if (!g_last_log_event.location) {
    return 11;
  }
  if (strcmp(g_last_log_event.location->function_name, "main")) {
    return 12;
  }
  if (g_last_log_event.location->line_number != 87u) {
    return 13;
  }
  if (g_last_log_event.severity != RCUTILS_LOG_SEVERITY_INFO) {
    return 14;
  }
  if (strcmp(g_last_log_event.name, "")) {
    return 15;
  }
  if (strcmp(g_last_log_event.message, "message foo")) {
    return 16;
  }

  rcl_logging_set_output_handler(previous_output_handler);
  if (g_last_log_event.message) {
    free(g_last_log_event.message);
  }
  g_rcl_logging_initialized = false;
  if (g_rcl_logging_initialized) {
    return 17;
  }
}
