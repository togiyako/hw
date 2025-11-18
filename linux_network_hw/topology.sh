#!/bin/bash

function create_lab() {

    ip netns add Netns1
    ip netns add Netns2
    ip netns add Netns3
    ip netns add Netns4
    ip netns add Netns5
    ip netns add Netns6

    ip link add Veth11 type veth peer name Veth15
    ip link set Veth11 netns Netns1
    ip link set Veth15 netns Netns5

    ip link add Veth22 type veth peer name Veth25
    ip link set Veth22 netns Netns2
    ip link set Veth25 netns Netns5

    ip link add Veth33 type veth peer name Veth36
    ip link set Veth33 netns Netns3
    ip link set Veth36 netns Netns6

    ip link add Veth44 type veth peer name Veth46
    ip link set Veth44 netns Netns4
    ip link set Veth46 netns Netns6

    ip link add Vethx type veth peer name Vethy
    ip link set Vethx netns Netns5
    ip link set Vethy netns Netns6

    ip netns exec Netns1 ip addr add 192.168.7.1/24 dev Veth11
    ip netns exec Netns1 ip link set Veth11 up
    ip netns exec Netns1 ip link set lo up

    ip netns exec Netns2 ip addr add 192.168.7.2/24 dev Veth22
    ip netns exec Netns2 ip link set Veth22 up
    ip netns exec Netns2 ip link set lo up

    ip netns exec Netns3 ip addr add 192.168.7.3/24 dev Veth33
    ip netns exec Netns3 ip link set Veth33 up
    ip netns exec Netns3 ip link set lo up

    ip netns exec Netns4 ip addr add 192.168.7.4/24 dev Veth44
    ip netns exec Netns4 ip link set Veth44 up
    ip netns exec Netns4 ip link set lo up

    ip netns exec Netns5 ip link add brx type bridge vlan_filtering 1
    ip netns exec Netns6 ip link add bry type bridge vlan_filtering 1

    ip netns exec Netns5 ip link set brx up
    ip netns exec Netns5 ip link set lo up
    ip netns exec Netns6 ip link set bry up
    ip netns exec Netns6 ip link set lo up

    ip netns exec Netns5 ip link set Veth15 master brx
    ip netns exec Netns5 ip link set Veth25 master brx
    ip netns exec Netns5 ip link set Vethx master brx
    ip netns exec Netns5 ip link set Veth15 up
    ip netns exec Netns5 ip link set Veth25 up
    ip netns exec Netns5 ip link set Vethx up

    ip netns exec Netns6 ip link set Veth36 master bry
    ip netns exec Netns6 ip link set Veth46 master bry
    ip netns exec Netns6 ip link set Vethy master bry
    ip netns exec Netns6 ip link set Veth36 up
    ip netns exec Netns6 ip link set Veth46 up
    ip netns exec Netns6 ip link set Vethy up

    ip netns exec Netns5 bridge vlan del dev Veth15 vid 1
    ip netns exec Netns5 bridge vlan add dev Veth15 vid 10 pvid untagged

    ip netns exec Netns5 bridge vlan del dev Veth25 vid 1
    ip netns exec Netns5 bridge vlan add dev Veth25 vid 20 pvid untagged

    ip netns exec Netns5 bridge vlan add dev Vethx vid 10
    ip netns exec Netns5 bridge vlan add dev Vethx vid 20

    ip netns exec Netns6 bridge vlan del dev Veth36 vid 1
    ip netns exec Netns6 bridge vlan add dev Veth36 vid 10 pvid untagged

    ip netns exec Netns6 bridge vlan del dev Veth46 vid 1
    ip netns exec Netns6 bridge vlan add dev Veth46 vid 20 pvid untagged

    ip netns exec Netns6 bridge vlan add dev Vethy vid 10
    ip netns exec Netns6 bridge vlan add dev Vethy vid 20
    
    echo "---All done---"
}

function clean_lab() {
    ip netns del Netns1 2>/dev/null
    ip netns del Netns2 2>/dev/null
    ip netns del Netns3 2>/dev/null
    ip netns del Netns4 2>/dev/null
    ip netns del Netns5 2>/dev/null
    ip netns del Netns6 2>/dev/null
}

case "$1" in
    create)
        create_lab
        ;;
    clear)
        clean_lab
        ;;
    *)
    	echo "Error: No action specified!"
        exit 1
        ;;
esac
