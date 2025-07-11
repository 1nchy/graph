#!/bin/bash

# Usage: mermaid.sh <cpp-file>
# Outputs markdown content to stdout.

set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <cpp-file>" >&2
  exit 1
fi

source=$1

inside_template=0
mermaid_title=""
buffer=""

mermaid_title_regex='^template[[:space:]]*<>[[:space:]]*auto ([^(]+)\(\) -> .*\{[[:space:]]*$'
mermaid_begin='^[[:space:]]*~~~mermaid;[[:space:]]*$'
mermaid_end='^[[:space:]]*~~~;[[:space:]]*$'

g++ -E "$source" -I include -DMERMAID | while IFS= read -r line; do
  if [[ $inside_template -eq 0 ]]; then
    if [[ $line =~ $mermaid_title_regex ]]; then
      mermaid_title="${BASH_REMATCH[1]}"
      inside_template=1
      buffer=""
      inside_mermaid=0
    fi
  else
    if [[ $inside_mermaid -eq 0 ]]; then
      if [[ $line =~ $mermaid_begin ]]; then
        inside_mermaid=1
        buffer="graph LR"
      fi
    else
      if [[ $line =~ $mermaid_end ]]; then
        echo "### $mermaid_title"
        echo
        echo '~~~mermaid'
        echo "$buffer"
        echo '~~~'
        echo

        inside_template=0
        mermaid_title=""
        buffer=""
        inside_mermaid=0
      else
        trimmed_line="${line#"${line%%[![:space:]]*}"}"
        buffer+=$'\n'"$trimmed_line"
      fi
    fi
  fi
done