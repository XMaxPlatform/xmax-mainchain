:man_page: mongoc_collection_watch

mongoc_collection_watch()
=========================

Synopsis
--------

.. code-block:: c

  mongoc_change_stream_t*
  mongoc_collection_watch (const mongoc_collection_t *coll,
                           const bson_t *pipeline,
                           const bson_t *opts);

A helper function to create a change stream. It is preferred to call this
function over using a raw aggregation to create a change stream.

This function uses the read preference and read concern of the collection. If
the change stream needs to re-establish connection, the same read preference
will be used. This may happen if the change stream encounters a resumable error.

.. warning::

   A change stream is only supported with majority read concern.

Parameters
----------

* ``coll``: A :symbol:`mongoc_collection_t` specifying the collection which the change stream listens to.
* ``pipeline``: A :symbol:`bson:bson_t` representing an aggregation pipeline appended to the change stream. This may be an empty document.
* ``opts``: A :symbol:`bson:bson_t` containing change stream options or ``NULL``.

``opts`` may be ``NULL`` or a document consisting of any subset of the following
parameters:

* ``batchSize`` An ``int32`` representing number of documents requested to be returned on each call to :symbol:`mongoc_change_stream_next`
* ``resumeAfter`` A ``Document`` representing the starting point of the change stream
* ``maxAwaitTimeMS`` An ``int64`` representing the maximum amount of time a call to :symbol:`mongoc_change_stream_next` will block waiting for data
* ``collation`` A `Collation Document <https://docs.mongodb.com/manual/reference/collation/>`_

Returns
-------
A newly allocated :symbol:`mongoc_change_stream_t` which must be freed with
:symbol:`mongoc_change_stream_destroy` when no longer in use. The returned
:symbol:`mongoc_change_stream_t` is never ``NULL``. If there is an error, it can
be retrieved with :symbol:`mongoc_change_stream_error_document`, and subsequent
calls to :symbol:`mongoc_change_stream_next` will return ``false``.