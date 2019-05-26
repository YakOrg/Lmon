#!/bin/bash

send_post() {
   for VARIABLE in {1..$1}
   do
        curl -d endpoint=$2_${VARIABLE} http://127.0.0.1:8080/agents/add
        echo
   done
}

for i in {1..100}
do
        send_post ${i} & # Put a function in the background
done