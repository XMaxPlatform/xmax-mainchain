:man_page: mongoc_collection_replace_one

mongoc_collection_replace_one()
===============================

Synopsis
--------

.. code-block:: c

  bool
  mongoc_collection_replace_one (mongoc_collection_t *collection,
                                 const bson_t *selector,
                                 const bson_t *replacement,
                                 const bson_t *opts,
                                 bson_t *reply,
                                 bson_error_t *error);

Parameters
----------

* ``collection``: A :symbol:`mongoc_collection_t`.
* ``selector``: A :symbol:`bson:bson_t` containing the query to match the document for updating.
* ``replacement``: A :symbol:`bson:bson_t` containing the replacement document.
* ``reply``: Optional. An uninitialized :symbol:`bson:bson_t` populated with the update result, or ``NULL``.
* ``error``: An optional location for a :symbol:`bson_error_t <errors>` or ``NULL``.

.. include:: includes/replace-one-opts.txt

Description
-----------

This function shall replace documents in ``collection`` that match ``selector`` with ``replacement``.

If you pass a non-NULL ``reply``, it is filled out with fields "modifiedCount" and "matchedCount". If there is a server error then ``reply`` contains either a "writeErrors" array with one subdocument or a "writeConcernErrors" array. The reply must be freed with :symbol:`bson:bson_destroy`.

See Also
--------

`MongoDB update command documentation <https://docs.mongodb.com/master/reference/command/update/>`_ for more information on the update options.

:symbol:`mongoc_collection_update_one`
:symbol:`mongoc_collection_update_many`

Errors
------

Errors are propagated via the ``error`` parameter.

Returns
-------

Returns ``true`` if successful. Returns ``false`` and sets ``error`` if there are invalid arguments or a server or network error.

A write concern timeout or write concern error is considered a failure.
