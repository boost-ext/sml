#!/usr/bin/env bash
#
# Regenerate the "Run on Compiler Explorer" links embedded in the docs.
#
# Boost.SML is a single-header library that is NOT registered as a Compiler
# Explorer library. Each godbolt.org/z/ link keeps the example source intact and
# pulls the header via Compiler Explorer's client-side "#include from URL" feature:
#   #include <https://raw.githubusercontent.com/boost-ext/sml/master/include/boost/sml.hpp>
# CE fetches and pastes that header in the browser when the link is opened (it is
# NOT resolved by the compile API). dispatch_table.hpp itself does a relative
# `#include "boost/sml.hpp"`, which CE's fetcher cannot resolve, so for the two
# examples that use it (dispatch_table, sdl2) the small utility header is inlined
# with its own sml.hpp include rewritten to the URL form.
#
# The error/* examples intentionally fail to compile and get compile-only links
# that show the diagnostic.
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
DT=include/boost/sml/utility/dispatch_table.hpp
URL='https://raw.githubusercontent.com/boost-ext/sml/master/include/boost/sml.hpp'

assemble() { # <src.cpp>  -> thin url-include source on stdout
  # Rewrite `#include <boost/sml.hpp>` to the URL form; for the dispatch_table.hpp
  # include, inline that small header with its own sml.hpp include rewritten too.
  awk -v url="$URL" -v dt="$DT" '
    /#include[ \t]*[<"]boost\/sml\.hpp[>"]/ { print "#include <" url ">"; next }
    /#include[ \t]*[<"]boost\/sml\/utility\/dispatch_table\.hpp[>"]/ {
        while ((getline l < dt) > 0)
          if (l ~ /#include[ \t]*[<"]boost\/sml\.hpp[>"]/) print "#include <" url ">"; else print l
        close(dt); next }
    { print }
  ' "$1"
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
  # strip previously-generated CE links, then re-insert after each embed.
  # Only delete a CE link that immediately FOLLOWS an ![CPP] embed, so standalone
  # links (e.g. the make_action example in the user guide) are preserved.
  DOCS=(doc/examples.md doc/tutorial.md doc/user_guide.md doc/index.md doc/overview.md)
  for d in "${DOCS[@]}"; do
    awk '{ if (prev ~ /!\[CPP/ && $0 ~ /Compiler Explorer\]\(https:\/\/godbolt\.org/) { prev=$0; next } print; prev=$0 }' "$d" > "$d.tmp" && mv "$d.tmp" "$d"
  done
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
