#!/usr/bin/env bash
# This checkout vendors dependencies; never contact a remote during a build.
set -eu
if [ "$#" -eq 0 ]; then
  echo "Vendored dependencies are checked by their build targets."
  exit 0
fi
for path in "$@"; do
  if [ ! -d "$path" ] || [ -z "$(ls -A "$path")" ]; then
    echo "Missing vendored dependency: $path" >&2
    exit 1
  fi
done
