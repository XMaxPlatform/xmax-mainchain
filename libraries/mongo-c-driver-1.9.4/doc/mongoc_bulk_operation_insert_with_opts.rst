:man_page: mongoc_bulk_operation_insert_with_opts

mongoc_bulk_operation_insert_with_opts()
========================================

Synopsis
--------

.. code-block:: c

  bool
  mongoc_bulk_operation_insert_with_opts (mongoc_bulk_operation_t *bulk,
                                          const bson_t *document,
                                          const bson_t *opts,
                                          bson_error_t *error); /* OUT */

Queue an insert of a single document into a bulk operation. The insert is not performed until :symbol:`mongoc_bulk_operation_execute()` is called.

Parameters
----------

* ``bulk``: A :symbol:`mongoc_bulk_operation_t`.
* ``document``: A :symbol:`bson:bson_t`.
* ``opts``: A :symbol:`bson:bson_t` containing additional options.
* ``error``: An optional location for a :symbol:`bson_error_t <errors>` or ``NULL``.

Currently the ``opts`` is unused.

Errors
------

Operation errors are propagated via :symbol:`mongoc_bulk_operation_execute()`, while argument validation errors are reported by the ``error`` argument.

Returns
-------

Returns true on success, and false if there is a server or network error or if passed invalid arguments.
