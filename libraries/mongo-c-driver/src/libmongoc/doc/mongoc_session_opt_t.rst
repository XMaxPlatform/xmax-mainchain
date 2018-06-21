:man_page: mongoc_session_opt_t

mongoc_session_opt_t
====================

.. code-block:: c

  #include <mongoc.h>

  typedef struct _mongoc_session_opt_t mongoc_session_opt_t;

Synopsis
--------

.. include:: includes/session-lifecycle.txt

See the example code for :symbol:`mongoc_session_opts_set_causal_consistency`.

.. only:: html

  Functions
  ---------

  .. toctree::
    :titlesonly:
    :maxdepth: 1

    mongoc_session_opts_new
    mongoc_session_opts_get_causal_consistency
    mongoc_session_opts_set_causal_consistency
    mongoc_session_opts_clone
    mongoc_session_opts_destroy
