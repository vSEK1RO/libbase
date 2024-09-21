#!/bin/bash

RESPONSE=$(curl -L \
-H "Accept: application/vnd.github+json" \
-H "X-GitHub-Api-Version: 2022-11-28" \
https://api.github.com/repos/vSEK1RO/libbasen/contributors)

LOGIN=$(echo "$RESPONSE" | sed -n /login/p | sed 's/    \"login\": \"//' | sed 's/\",//')
CONTRIBUTIONS=$(echo "$RESPONSE" | sed -n /contributions/p | sed 's/    \"contributions\": //' | sed 's/,//')

IFS=$'\n' read -r -d '' -a arr1 <<< "$CONTRIBUTIONS"
IFS=$'\n' read -r -d '' -a arr2 <<< "$LOGIN"

AUTHORS=$'libbasen project authors:\n\n'
AUTHORS+=$(printf "%7s | %s" "commits" "username")$'\n'
for ((i = 0; i < ${#arr1[@]}; i++)); do
  AUTHORS+=$(printf "%7s | %s" "${arr1[$i]}" "${arr2[$i]}")$'\n'
done

echo "$AUTHORS" > AUTHORS