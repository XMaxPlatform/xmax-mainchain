# -*- coding: utf-8 -*-
import os.path
import sys

# Ensure we can import "mongoc" and "taglist" extension modules.
this_path = os.path.dirname(__file__)
sys.path.append(this_path)
sys.path.append(os.path.normpath(os.path.join(this_path, '../../../build/sphinx')))

from mongoc_common import *

extensions = [
    'mongoc',
    'taglist',
    'sphinx.ext.intersphinx',
    'sphinx.ext.extlinks',
]

# General information about the project.
project = 'MongoDB C Driver'
copyright = '2017-present, MongoDB, Inc'
author = 'MongoDB, Inc'

version_path = os.path.join(
    os.path.dirname(__file__), '../../..', 'VERSION_CURRENT')
version = open(version_path).read().strip()
release_path = os.path.join(
    os.path.dirname(__file__), '../../..', 'VERSION_RELEASED')
release = open(release_path).read().strip()
release_major, release_minor, release_patch = release.split('.')
release_download = 'https://github.com/mongodb/mongo-c-driver/releases/download/{0}/mongo-c-driver-{0}.tar.gz'.format(release)
rst_prolog = """
.. |release_major| replace:: %(release_major)s

.. |release_minor| replace:: %(release_minor)s

.. |release_patch| replace:: %(release_patch)s

.. |release_download| replace:: https://github.com/mongodb/mongo-c-driver/releases/download/%(release)s/mongo-c-driver-%(release)s.tar.gz
""" % locals()

# The extension requires the "base" to contain '%s' exactly once, but we never intend to use it though
extlinks = {'release': (release_download+'%s', '')}

language = 'en'
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
master_doc = 'index'

# don't fetch libbson's inventory from mongoc.org during build - Debian and
# Fedora package builds must work offline - maintain a recent copy here
intersphinx_mapping = {
    'bson': ('http://mongoc.org/libbson/current', 'libbson-objects.inv'),
}

# -- Options for HTML output ----------------------------------------------

html_theme_path = ['../../../build/sphinx']
html_theme = 'mongoc-theme'
html_title = html_shorttitle = 'MongoDB C Driver %s' % version
# html_favicon = None

html_sidebars = {
    '**': ['globaltoc.html'],
    'errors': [],  # Make more room for the big table.
    'mongoc_uri_t': [],  # Make more room for the big table.
}


def add_canonical_link(app, pagename, templatename, context, doctree):
    link = ('<link rel="canonical"'
            ' href="http://mongoc.org/libbson/current/%s.html"/>' % pagename)

    context['metatags'] = context.get('metatags', '') + link


def setup(app):
    mongoc_common_setup(app)
    app.connect('html-page-context', add_canonical_link)
