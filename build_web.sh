#!/bin/bash

set -e

source ./emsdk/emsdk_env.sh

rm -rf build-web

emcmake cmake -B build-web
cmake --build build-web

cp ./build-web/night_city.html ./index.html
cp ./build-web/night_city.js .
cp ./build-web/night_city.wasm .
