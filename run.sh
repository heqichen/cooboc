#!/bin/bash

sleep 4

/home/heqichen/Faurecia/git/chicken/chicken /tmp/test.json &

cd /home/heqichen/Faurecia/git/node_show
node index.js 8081 /tmp/test.json

