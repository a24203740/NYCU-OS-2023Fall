#!/bin/bash

# Read parent pid, child pid

SHOW_PATH=false

while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      SHOW_PATH=true
      shift 1
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

########
# TODO #
########

declare -a PID_LIST
CURRENT_PID=$child
PID_LIST+=(${child})

while [ -n "${CURRENT_PID}" ] && [ ${CURRENT_PID} -ne 1 ] && [ ${CURRENT_PID} -ne ${parent} ]; do

  CURRENT_PID=$(ps ax -q ${CURRENT_PID} o pid,ppid | grep ${CURRENT_PID} | tr -s ' ' | sed 's/^[ \t]*//' | cut -d ' ' -f 2)
  PID_LIST+=(${CURRENT_PID})
done

if [ -z "${CURRENT_PID}" ]; then
  echo "No"
elif [ ${CURRENT_PID} -eq ${parent} ]; then
  echo "Yes"
  if [ ${SHOW_PATH} = true ]; then
    len=${#PID_LIST[*]}
    for (( i=len-1; i>=0; i=i-1)); do
      echo -n "${PID_LIST[$i]}"
      if [ $i -gt 0 ]; then
        echo -n " -> "
      fi
    done
    echo ""
  fi
else
  echo "No"
fi