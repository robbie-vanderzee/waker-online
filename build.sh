#!/bin/bash

usage(){
  echo ""
  echo "Usage: $0 -premake flags -debug -release -clean -style -log"
  echo -e "\t-premake flags = flag1,flag2,flag3 is a list of flags to build premake with."
  echo -e "\t-debug builds debug configuration"
  echo -e "\t-release builds release configuration"
  echo -e "\t-clean purges the current build."
  echo -e "\t-style applies styling to the project source."
  echo -e "\t-log logs the build output and error to file"
  echo ""
}

tee() {
  for flag in "$@"; do
    if [ "$flag" != "${flag%/*}" ]; then
      mkdir -p "${flag%/*}";
    fi
  done
  command tee "$@";
}

debug=false
release=false
clean=false
style=false
logging=false

while getopts "p:drcsl" opt; do
   case $opt in
   p) set -f
      IFS=,
      premake=${OPTARG};;
   d) debug=true;;
   r) release=true;;
   c) clean=true;;
   s) style=true;;
   l) logging=true;;
   * ) usage
       exit 1;;
   esac
done

build(){
  if [ "$style" = true ] ; then
    mapfile -t files < <(find . -path ./waker-online/includes -prune -false -o -type f -iname \*\.cpp -o -iname \*\.hpp -o -iname \*\.c -o -iname \*\.h)
    if [ "$logging" = true ] ; then
      for file in "${files[@]}" ; do
        astyle --style=google --suffix=none -N -L -w -U -W2 -c -S -p -H --indent=spaces -p "$file" | tee logging/build/style.log
      done
    else
      for file in "${files[@]}" ; do
        astyle --style=google --suffix=none -N -L -w -U -W2 -c -S -p -H --indent=spaces -p "$file" | tee logging/build/style.log
      done
    fi
  fi
  if [ "$logging" = true ] ; then
    premake5 gmake2 "${premake}" |& tee logging/build/debug.log logging/build/release.log
  else
    premake5 gmake2 "${premake}"
  fi
  if [ "$clean" = true ] ; then
    if [ "$debug" = true ] ; then
      make clean config=debug
    fi
    if [ "$release" = true ]; then
      make clean config=release
    fi
  fi
  if [ "$logging" = true ] ; then
    if [ "$debug" = true ] ; then
      make -j config=debug |& tee -a logging/build/debug.log
    fi
    if [ "$release" = true ] ; then
      make -j config=release |& tee -a logging/build/release.log
    fi
  else
    if [ "$debug" = true ] ; then
      make config=debug
    fi
    if [ "$release" = true ] ; then
      make config=release
    fi
  fi
  echo "Build complete."
}

build
