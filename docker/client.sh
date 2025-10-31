#!/bin/sh

sleep 5

echo "Sending massage to server"

echo "Hello from client" | nc -q 0 server-container 7000

echo "Massage sent"
