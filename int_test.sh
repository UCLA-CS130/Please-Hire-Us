#!/bin/bash

#Run clean version
make clean
make

#Run the server
./server config > /dev/null &
echo "Running server."

sleep 2

#Check for multithreading
telnet localhost 8008 > /dev/null &

#Send message to server
ACTUAL_MSG="$(curl -si http://localhost:8080/echo)"

#Verify expected message
EXPECTED_MSG="HTTP/1.1 200\r\nContent-Type: text/plain\r\n\r\nGET /echo HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: localhost:8080\r\nAccept: */*\r\n\r"

echo -ne "$ACTUAL_MSG" | hexdump -C > tmp1
echo -ne "$EXPECTED_MSG" | hexdump -C > tmp2

#Check that messages are equal
if cmp -s "tmp1" "tmp2"
then
    echo "Server test succeeded. Killing server process."
else
    echo "Server test failed. Killing server process."
fi 

#Remove temporary files
rm tmp1
rm tmp2

#Kill server
kill -9 %1 
