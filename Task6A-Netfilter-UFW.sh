sudo ip netns delete vnet0 2>/dev/null
sudo ip link delete veth0 2>/dev/null
sudo ip link delete veth01 2>/dev/null
sum rm -f /run/netns/vnet0
sudo ufw disable
sudo ufw reset

sudo ip netns add vnet0
sudo ip link add dev veth01 peer name veth02 netns vnet0
sudo ip link set up veth01
sudo ip netns exec vnet0 ip link set up veth02
sudo ip addr add 10.168.0.1/24 dev veth01
sudo ip netns exec vnet0 ip addr add 10.168.0.2/24 dev veth02
sudo ip netns exec vnet0 nc -l -p 9999 #COMMUNICATION: POSSIBLE

sudo ufw status #Inactive
sudo ip netns exec vnet0 ufw status #Inactive
sudo ip netns exec vnet0 ufw enable
sudo ip netns exec vnet0 ufw status #Active
sudo ufw status #Inactive

sudo ip netns exec vnet0 iptables -L
