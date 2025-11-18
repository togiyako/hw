## About project ##
This project contains a Bash script for automatically deploying a virtual network topology in a Linux environment.

## Run and clear ##
First, you need to add permission for the file:
    chmod +x topology.sh

Command for execution:
    sudo ./topology.sh create

Command for clean setup:
    sudo ./topology.sh clear

## Tests ##
To test the topology, you need to execute these commands:
    sudo ip netns exec Netns1 ping -c 3 192.168.7.3
    
    sudo ip netns exec Netns2 ping -c 3 192.168.7.4
    
    sudo ip netns exec Netns1 ping -c 3 192.168.7.2
