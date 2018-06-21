/*
 * Copyright 2018-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JSON_TEST_MONITORING_H
#define JSON_TEST_MONITORING_H

#include "TestSuite.h"

#include <bson.h>
#include <mongoc.h>

#include "json-test-operations.h"

typedef void (*json_test_events_check_cb_t) (const bson_t *events);

void
set_apm_callbacks (mongoc_client_t *client, void *ctx);

void
check_json_apm_events (const bson_t *events, const bson_t *expectations);

#endif
