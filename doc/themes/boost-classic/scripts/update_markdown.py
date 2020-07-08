#!/usr/bin/env python
#update_md.py . http://boost-ext.github.io/sml/master

import fileinput, sys, os, glob

def print_cpp(filename):
  print("```cpp")
  with open(filename, 'r') as file:
    ignore = False
    tmp_ignore = False
    for line in file:
      if line.startswith("//<-"):
        ignore = True
        tmp_ignore = True
      elif line.startswith("//->"):
        tmp_ignore = False
      if not ignore and not line.startswith("//") and not line.find("clang-format") != -1:
        print line,
      ignore = tmp_ignore
  print("```")

def update_md(filename):
  for line in fileinput.input(filename, inplace=True):
    if line.find("[CPP]") != -1 or line.find("[CPP*]") != -1:
      print_cpp("../" + line.split("(")[1][len(sys.argv[2])+1:].split(")")[0])
      href = line.split("(")[1].split(")")[0]
      print "* [" + href + "](" + href + ")"
    elif line.find("[CPP(BTN)]") != -1:
      name = line.split("(")[2].split("|")[0].replace("_", " ").replace("Run", "")
      href = line.split("|")[1].split(")")[0]
      print "* [" + name + "](" + href + ")"
    elif line.find("[CPP(SHOW)]") != -1:
      print_cpp("../" + line.split("(")[2][len(sys.argv[2])+1:].split(")")[0])
    elif line.find("[CPP(SPLIT)]") != -1:
      print_cpp("../" + line.split("(")[2][len(sys.argv[2])+1:].split(")")[0])
    elif line.find("[IFRAME]") != -1:
      src = line.split("(")[1].split(")")[0]
      print "[" + src + "](" + src + ")"
    elif line.find("[YOUTUBE]") != -1:
      src = line.split("(")[1].split(")")[0]
      print "[" + src + "](" + src + ")"
    else:
      print(line),

for filename in glob.iglob(sys.argv[1] + '/*.md'):
  update_md(filename)
