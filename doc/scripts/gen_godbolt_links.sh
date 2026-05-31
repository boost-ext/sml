#!/usr/bin/env bash
#
# Regenerate the "Run on Compiler Explorer" links embedded in the docs.
#
# Boost.SML is a single-header library that is NOT registered as a Compiler
# Explorer library, so each godbolt.org/z/ link inlines include/boost/sml.hpp
# (and utility/dispatch_table.hpp where used) ahead of the example source.
# Any `#define BOOST_SML_*` config macro is hoisted above the inlined header so
# it still takes effect (see data.cpp / BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS).
#
# All examples are validated to compile (and execute) on GCC 14.2 at -std=c++14
# before a short link is created. The error/* examples intentionally fail to
# compile and get compile-only links that show the diagnostic.
#
# Requirements: bash, curl, jq. Run from the repository root.
# Usage:
#   doc/scripts/gen_godbolt_links.sh            # print "name url" for every example
#   doc/scripts/gen_godbolt_links.sh --insert   # also rewrite the CE links in doc/*.md
#
set -euo pipefail
cd "$(git rev-parse --show-toplevel)"

GCC=g142                      # Compiler Explorer GCC 14.2
STD_DEFAULT=c++14             # SML's minimum standard
HDR=include/boost/sml.hpp
DT=include/boost/sml/utility/dispatch_table.hpp
STRIP='#include[[:space:]]*[<"]boost/sml\.hpp[>"]|#include[[:space:]]*[<"]boost/sml/utility/dispatch_table\.hpp[>"]|^[[:space:]]*#define[[:space:]]+BOOST_SML'

assemble() { # <src.cpp>  -> self-contained translation unit on stdout
  local ex="$1"
  grep -E '^[[:space:]]*#define[[:space:]]+BOOST_SML' "$ex" || true   # hoist config macros
  cat "$HDR"
  grep -q 'utility/dispatch_table.hpp' "$ex" && grep -vE "$STRIP" "$DT"
  grep -vE "$STRIP" "$ex"
}

shorten() { # <assembled-file> <std> <execute:true|false> -> godbolt short url
  local src="$1" std="$2" exec="$3"
  jq -n --rawfile s "$src" --arg std "$std" --argjson ex "$exec" \
    '{sessions:[{id:1,language:"c++",source:$s,
      compilers:[{id:"'"$GCC"'",options:("-O2 -std="+$std),libs:[],
        filters:{execute:$ex,labels:true,directives:true,commentOnly:true,trim:true,demangle:true}}],
      executors:(if $ex then [{compiler:{id:"'"$GCC"'",options:("-O2 -std="+$std),libs:[],overrides:[]},
        arguments:"",stdin:"",compilerOutputVisible:true,stdoutVisible:true,stderrVisible:true}] else [] end)}]}' \
    | curl -s -X POST 'https://godbolt.org/api/shortener' -H 'Content-Type: application/json' -d @- | jq -r '.url'
}

MAP=$(mktemp)
process() { # <src.cpp> <execute:true|false>
  local ex="$1" exec="$2" name std tmp url
  name=$(basename "$ex" .cpp)
  std=$STD_DEFAULT; [ "$name" = arduino ] && std=c++20   # template lambdas
  tmp=$(mktemp); assemble "$ex" > "$tmp"
  url=$(shorten "$tmp" "$std" "$exec"); rm -f "$tmp"
  echo "$name $url" | tee -a "$MAP"
}

for f in example/*.cpp; do process "$f" true; done
for f in test/ft/errors/not_*.cpp; do process "$f" false; done

if [ "${1:-}" = "--insert" ]; then
  # strip previously-generated CE links, then re-insert after each embed
  DOCS=(doc/examples.md doc/tutorial.md doc/user_guide.md doc/index.md doc/overview.md)
  for d in "${DOCS[@]}"; do sed -i '/Compiler Explorer\](https:\/\/godbolt.org/d' "$d"; done
  while read -r name url; do
    case "$name" in
      not_*) label="▶ See the compile error on Compiler Explorer";;
      *)     label="▶ Run on Compiler Explorer";;
    esac
    for d in "${DOCS[@]}"; do sed -i "\#/${name}\.cpp)#a [$label]($url)" "$d"; done
  done < "$MAP"
  echo "docs updated."
fi
rm -f "$MAP"
