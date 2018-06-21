/*
 * Copyright 2018-present MongoDB Inc.
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

#ifndef BSON_B64_PRIVATE_H
#define BSON_B64_PRIVATE_H

#if !defined(MONGOC_COMPILATION) && !defined(BSON_COMPILATION) && \
   !defined(BSON_INSIDE)
#error "Only <mongoc.h> or <bson.h> can be included directly."
#endif

#include <bson.h>

int
bson_b64_ntop (uint8_t const *src,
               size_t srclength,
               char *target,
               size_t targsize);

int
bson_b64_pton (char const *src, uint8_t *target, size_t targsize);

#endif /* BSON_B64_PRIVATE_H */
