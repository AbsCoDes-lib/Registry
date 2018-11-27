#!/bin/sh

# Docs folder
DOCS=docs

# Doxyfile
DOXYFILE=$DOCS/Doxyfile


# Remove html folder
if [ -d "html" ]; then
    echo "Removing html folder ..."
    rm -rf html
fi

# Need to change the folder root before lauching docygen
cd ..

# Generate documentation
if [ -f "$DOXYFILE" ]; then
    echo "Generating Documentation ..."
    (cat "$DOXYFILE" ; echo OUTPUT_DIRECTORY="$DOCS") | doxygen -
fi
