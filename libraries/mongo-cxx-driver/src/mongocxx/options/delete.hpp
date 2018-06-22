// Copyright 2014 MongoDB Inc.
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

#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <mongocxx/write_concern.hpp>

#include <mongocxx/config/prelude.hpp>

namespace mongocxx {
MONGOCXX_INLINE_NAMESPACE_BEGIN
namespace options {

///
/// Class representing the optional arguments to a MongoDB delete operation
///
class MONGOCXX_API delete_options {
   public:
    ///
    /// Sets the collation for this operation.
    ///
    /// @param collation
    ///   The new collation.
    ///
    /// @return
    ///   A reference to the object on which this member function is being called.  This facilitates
    ///   method chaining.
    ///
    /// @see
    ///   https://docs.mongodb.com/master/reference/collation/
    ///
    delete_options& collation(bsoncxx::document::view_or_value collation);

    ///
    /// Retrieves the current collation for this operation.
    ///
    /// @return
    ///   The current collation.
    ///
    /// @see
    ///   https://docs.mongodb.com/master/reference/collation/
    ///
    const stdx::optional<bsoncxx::document::view_or_value>& collation() const;

    ///
    /// Sets the write_concern for this operation.
    ///
    /// @param wc
    ///   The new write_concern.
    ///
    /// @return
    ///   A reference to the object on which this member function is being called.  This facilitates
    ///   method chaining.
    ///
    /// @see https://docs.mongodb.com/master/core/write-concern/
    ///
    delete_options& write_concern(write_concern wc);

    ///
    /// The current write_concern for this operation.
    ///
    /// @return
    ///   The current write_concern.
    ///
    /// @see https://docs.mongodb.com/master/core/write-concern/
    ///
    ///
    const stdx::optional<class write_concern>& write_concern() const;

   private:
    stdx::optional<bsoncxx::document::view_or_value> _collation;
    stdx::optional<class write_concern> _write_concern;

    friend MONGOCXX_API bool MONGOCXX_CALL operator==(const delete_options&, const delete_options&);
    friend MONGOCXX_API bool MONGOCXX_CALL operator!=(const delete_options&, const delete_options&);
};

}  // namespace options
MONGOCXX_INLINE_NAMESPACE_END
}  // namespace mongocxx

#include <mongocxx/config/postlude.hpp>
