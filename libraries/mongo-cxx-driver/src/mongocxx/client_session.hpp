// Copyright 2017-present MongoDB Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>

#include <bsoncxx/document/view.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <mongocxx/options/client_session.hpp>

#include <mongocxx/config/prelude.hpp>

namespace mongocxx {
MONGOCXX_INLINE_NAMESPACE_BEGIN

class client;

///
/// Use a session for a sequence of operations, optionally with causal consistency.
///
/// Note that client_session is not thread-safe. See
/// https://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/thread-safety/ for more details.
///
/// @see http://dochub.mongodb.org/core/causal-consistency
///
class MONGOCXX_API client_session {
   public:
    ///
    /// Move constructs a session.
    ///
    client_session(client_session&&) noexcept;

    ///
    /// Move assigns a session.
    ///
    client_session& operator=(client_session&&) noexcept;

    client_session(const client_session&) = delete;
    client_session& operator=(const client_session&) = delete;

    ///
    /// Ends and destroys the session.
    ///
    ~client_session() noexcept;

    ///
    /// Gets the client that started this session.
    ///
    const class client& client() const noexcept;

    ///
    /// Gets the options this session was created with.
    ///
    const options::client_session& options() const noexcept;

    ///
    /// Get the server-side "logical session ID" associated with this session, as a BSON document.
    /// This view is invalid after the session is destroyed.
    ///
    bsoncxx::document::view id() const noexcept;

    ///
    /// Get the session's clusterTime, as a BSON document. This is an opaque value suitable for
    /// passing to advance_cluster_time(). The document is empty if the session has
    /// not been used for any operation and you have not called advance_cluster_time().
    /// This view is invalid after the session is destroyed.
    ///
    bsoncxx::document::view cluster_time() const noexcept;

    ///
    /// Get the session's operationTime, as a BSON timestamp. This is an opaque value suitable for
    /// passing to advance_operation_time(). The timestamp is zero if the session has not been used
    /// for any operation and you have not called advance_operation_time().
    ///
    bsoncxx::types::b_timestamp operation_time() const noexcept;

    ///
    /// Advance the cluster time for a session. Has an effect only if the new cluster time is
    /// greater than the session's current cluster time.
    ///
    /// Use advance_operation_time() and advance_cluster_time() to copy the operationTime and
    /// clusterTime from another session, ensuring subsequent operations in this session are
    /// causally consistent with the last operation in the other session.
    ///
    void advance_cluster_time(const bsoncxx::document::view& cluster_time);

    ///
    /// Advance the session's operation time, expressed as a BSON timestamp. Has an effect only if
    /// the new operation time is greater than the session's current operation time.
    ///
    /// Use advance_operation_time() and advance_cluster_time() to copy the operationTime and
    /// clusterTime from another session, ensuring subsequent operations in this session are
    /// causally consistent with the last operation in the other session.
    ///
    void advance_operation_time(const bsoncxx::types::b_timestamp& operation_time);

   private:
    friend class bulk_write;
    friend class client;
    friend class collection;
    friend class database;
    friend class index_view;

    class MONGOCXX_PRIVATE impl;

    // "class client" distinguishes from client() method above.
    MONGOCXX_PRIVATE client_session(const class client* client,
                                    const options::client_session& options);

    MONGOCXX_PRIVATE impl& _get_impl();
    MONGOCXX_PRIVATE const impl& _get_impl() const;

    std::unique_ptr<impl> _impl;
};

MONGOCXX_INLINE_NAMESPACE_END
}  // namespace mongocxx

#include <mongocxx/config/postlude.hpp>

///
/// @example examples/mongocxx/client_session.cpp
/// Use a mongocxx::client_session for a sequence of operations with causal consistency.
///
