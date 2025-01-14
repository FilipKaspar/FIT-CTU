# <p style="text-align:center;"><b>PSI</b></p>

### Basics

![osi](images/osi.jpg)
![osi](images/comm.JPG)

**Topology**

![topology](images/topology.JPG)

**Types of connection (hardware level)**

![conn](images/conn.JPG)

### Different types of communicational operations

![operations](images/operations.JPG)

**MAC address**

![mac](images/mac.JPG)

**Multiplex**

![multiplex](images/multiplex.JPG)

### Configurating the stream of data flow

1. Single confirming

![single](images/single.JPG)

2. Continual confirming with selective repeating 

![continual](images/select.JPG)

3. Continual confirming with return

![return](images/continaul.JPG)

4. Sliding window

![slide](images/slide.JPG)

### How VLAN works
Virtual Local Area Network

![vlan](images/vlan.JPG)

Trunk port = port, který odbavuje provoz více různých VLAN

**How do we differentaite VLANs**

![dif](images/divide.JPG)
![ex](images/ex.JPG)
![ex2](images/ex2.JPG)

**IPv4 Packet description**

![packet](images/packet.JPG)
![packet_info](images/packetInfo.JPG)

### Process of routing

**Example of routing**

![rout](images/rou_table.JPG)
![rout_info](images/rout_info.JPG)
![rout_des](images/rout_des.JPG)

### Fragmentation of IP packets

![frag_ex](images/frag_ex.JPG)
![frag_info](images/frag_info.JPG)

### NAT - Netwok Address Translation

![nat](images/nat.JPG)

### ICMP - Internet Control Message Protocol

![icmp](images/icmp.JPG)

### ARP - Address Resolution Protocol

![arp_info](images/arp_info.JPG)
![arp_ex](images/arp_ex.JPG)
![arp_spoof](images/arp_spoof.JPG)
![arp_proxy](images/arp_proxy.JPG)

### Dynamic konfiguration of IPv4

![dhcp](images/dhcp.JPG)

### Example of configuring LAN and subnets

![lan](images/subnets_ex.JPG)

### IPv6 and how it works

![packet](images/ip6packet.JPG)

**Types of ip address in IPv6**

![types](images/types.JPG)

**ND - Neighbor discovery**

![nd](images/nd.JPG)

**How to get MAC address from an IPv6 address**

![iptomac](images/iptomac.JPG)

**Finding link addresses in LAN**

![macinfo](images/ipv6mac_info.JPG)

**IPv6 summary**

![summary](images/ipv6_summary.JPG)

**DHCPv6 is very similar to how DHCPv4 works**

**How IPv6 connect to IPv4 network**

![nat64](images/nat64.JPG)

### Types of routing

![routing](images/routing.JPG)

### Routing tables + examples

![rout](images/rout.JPG)
![vs](images/routingvs.JPG)

**Dynamic vs static configuration of routinig tables**
**Static** - Has to be set and maintained manually. Good for smaller net structures. Works immediately after plugging the device to the network.  
**Dynamic** - The tables are set and maintained by routers automatically. Thanks to quick periodic requesting, are changes in network updated very quickly.  
**3 Types**

![dynamic](images/dynamic.JPG)

### Routing Information protocol

![rip](images/rip.JPG)

**When adding a new router, every routing table in network has to be updated**

![ripupdated](images/ripup.JPG)

**AS** - Autononomous system
**BGP** - Border Gateway Protocol
**Path to BGP** - Sequence in which are AS setup from the source to the final destination. 
![bgp](images/bgp.JPG)
![bgp1](images/bgp1.JPG)
![bgp2](images/bgp_steps.JPG)

**Example of the routing BGP table**
![bgp_ex](images/bgp_ex.JPG)

### TCP and UDP

**Parameters**

![parameters](images/parameters.JPG)

**Setting up connection - 3 steps**
1. Sending info (trying to connect)
2. Sending info back (answer about connection)
3. Sending info again (confirming the answer)

**How TCP connection works**
![tcp](images/tcp.JPG)
![tcp_life](images/tcp_life.JPG)

**Line congestion**
![congestion](images/congestion.JPG)
CWL - Maximum amount of packets that can be send at the same time, to prevent overloading the connection 

### UDP

![udp](images/udp.JPG)

**Address translation in NAT**
![address](images/address.JPG)

### DNS - Domain name system

![dns](images/dns.JPG)
![dns_hi](images/dns_hi.JPG)

**What is it used for?**
![dns_des](images/dns_des.JPG)
\+ Load Balancing
![dns_packet](images/dns_packet.JPG)

1. Iterative
2. Recursive
![dns_rout](images/dns_routing.JPG)

1. Dynamic DNS - DynDNS
2. Safer DNS - DNSSEC

![dnssec](images/dnssec.JPG)
![dns_poisoning](images/dns_poisoning.JPG)

### Security in the network

![firewall](images/firewall.JPG)
![types](images/firewall_types.JPG)
![packet](images/firewall_packet.JPG)

**DPI - Deep Packet Inspection**
![dpi](images/DPI.JPG)

**Proxy - multiple usage**
1. Hiding source IP
2. Cacheing websites
3. Authorizing users

![proxy](images/proxy.JPG)

### Detection systems and how they handle packets

1. **IDS - Intrusion Detection System** - only detection  
![ids](images/ids.JPG)

2. **IPS - Intrusion Prevention system** - detection and elimination
![ids](images/ips.JPG)

3. **Honeypot**
![honey](images/honeypot.JPG)

### Typical attacks over the newtwork

1. **Scanning Ports**  
Discovering which applications are running on different ports.
    - Horizontal - Scanning single port with different IP addresses
    - Vertical - Scanning multiple ports at once with same IP address  
**Defence** - Firewall, DS and IPS    

2. **Cracking passwords**  
Attack is being executed through terminal services such as SSH, RDP, TELNET.
    - Brute Force - brute force
    -  Dictionary Attack - trying passwards from usually a list of most common passwords
**Defence** - Firewall and IPS

3. **DOS**
    - DOS - Denial Of Service
    - DDOS - Distributed Denial Of Service
    ![ddos](images/ddos.JPG)
    - RDOS - IP address is being spoofed
    ![rdos](images/RDOS.JPG)  
    **Defence** - IPS

4. **Attacks that are using specific vulnerability in specific system**
    - Worms
    - Buffer Overflow - leakz  
    **Defence** - patches for current system

5.  **Spam**
    - Phishing - false website, goal = getting users credentials

6. **Malware**  
App that is running on the background of the OS without user knowing
    - Trojan Horse - seems like legitness app
    - Ransomware - Crypting the hard drive, making unpossible to access the data
    - Spyware - based on the configuration of the OS, trying to steal user's information
    - Adware - Just ADs

### VPN - Virtual Private Network

**Types of VPNs**
![vpn_types](images/vpn_types.JPG)

**How VPNs work?**

![vpn](images/vpn.JPG)
![tunnel](images/tunneling.JPG)

**Comparison without and with VPN**

![comparison](images/vpn_comp.JPG)

**Encapsulating data while transfering them through VPN**
![vpn_packet](images/vpn_packet.JPG)

**PPP - point to point protocol**
- Support compresion and encryption
- Connection is established only on 2 sides
![ppp](images/ppp.JPG)

**IPSec - IPSecured**
![ipsec](images/ipsec.JPG)

![tunel](images/ipsec_tunel.JPG)

**IP over IP protocol**
![ipoip](images/ipoip.JPG)

**GRE - Generic Routing Encapsulation**
![gre](images/gre.JPG)
![pptp](images/pptp.JPG)
![l2tp](images/l2tp.JPG)

**SSTP - Secure Socket Tunneling Protocol**

![sstp](images/sstp.JPG)

**Open VPN**
![openvpn](images/open_vpn.JPG)
![openvpn_schema](images/openvpn_schema.JPG)
