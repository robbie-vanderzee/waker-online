#!/bin/bash

usage(){
  echo ""
  echo "Usage: $0 -premake flags -debug -release -clean -style -log"
  echo -e "\t-[p]remake flags = flag1,flag2,flag3 is a list of flags to build premake with."
  echo -e "\t-[d]ebug builds debug configuration."
  echo -e "\t-[r]elease builds release configuration."
  echo -e "\t-[c]lean purges the current build."
  echo -e "\t-[s]tyle applies styling to the project source."
  echo -e "\t-[l]og logs the build output and error to file."
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

run_style(){
  if [ "$style" = true ] ; then
    mapfile -t files < <(find . -path ./waker-online/includes -prune -false -o -type f -iname \*\.cpp -o -iname \*\.hpp -o -iname \*\.c -o -iname \*\.h)
    if [ "$logging" = true ] ; then
      for file in "${files[@]}" ; do
        unbuffer astyle --style=google --suffix=none -N -L -w -U -W2 -c -S -p -H --indent=spaces -p "$file" | tee -a logging/build/style.log
      done
    else
      for file in "${files[@]}" ; do
        unbuffer astyle --style=google --suffix=none -N -L -w -U -W2 -c -S -p -H --indent=spaces -p "$file" | tee -a logging/build/style.log
      done
    fi
  fi
}

run_premake5(){
  if [ "$logging" = true ] ; then
    unbuffer premake5 gmake2 "${premake}" |& tee logging/build/debug.log logging/build/release.log
  else
    unbuffer premake5 gmake2 "${premake}"
  fi
}

run_make_clean(){
  if [ "$clean" = true ] ; then
    if [ "$debug" = true ] ; then
      unbuffer make clean config=debug
    fi
    if [ "$release" = true ]; then
      unbuffer make clean config=release
    fi
  fi
}

run_make_build(){
  if [ "$logging" = true ] ; then
    if [ "$debug" = true ] ; then
      unbuffer make -j config=debug |& tee -a logging/build/debug.log
    fi
    if [ "$release" = true ] ; then
      unbuffer make -j config=release |& tee -a logging/build/release.log
    fi
  else
    if [ "$debug" = true ] ; then
      unbuffer make config=debug
    fi
    if [ "$release" = true ] ; then
      unbuffer make config=release
    fi
  fi
}

debug=false
release=false
clean=false
style=false
logging=false

while getopts "p:drcslh" opt; do
   case $opt in
   p) set -f
      IFS=,
      premake=${OPTARG};;
   d) debug=true;;
   r) release=true;;
   c) clean=true;;
   s) style=true;;
   l) logging=true;;
   h) usage
      exit 0;;
   * ) usage
       exit 1;;
   esac
done


build(){
  run_style
  run_premake5
  run_make_clean
  run_make_build
  echo "Build complete."
}


build
