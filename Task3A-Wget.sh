# ENSURE ISO FILE IS FITTED SUCCESSFULLY IN VIRTUALBOX
mkdir etc
echo "nameserver 8.8.8.8" > /etc/resolv.conf
ip link set up dev eth0
ifconfig eth0 up
ip addr
ip route
ip addr show dev eth0
ip addr add 10.0.2.15/24 dev eth0
ip link set eth0 up
ip route add default via 10.0.2.2 dev eth0
ping -c 3 google.com
