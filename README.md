# ft_ping
A recreation in C of the ping implementation from inetutils-2.0 (ping -V)

## Workflow
#### Parse arguments:

> Identify the destination (hostname/IP) and any optional flags.<br>
> The mandatory part has to handle `-v` and `-?`. Other flags handled count as bonus.

#### Resolve hostname:

> Convert the hostname to an IP address for use with sockets. <br>
> Once the IP is obtained, it is necessary to reverse lookup the IP to obtain the Full Qualified Domain Name (FQDN).

#### Open socket:

> Create a raw socket for sending and receiving ICMP packets.<br>
> This requires root-permission, so the program must be run with `sudo`.

#### Main loop:

> Send ICMP echo requests and receive replies.<br>
> Track statistics (sent, received, duplicates, etc.).

#### Exit/cleanup:

> On SIGINT or after sending the specified number of packets, call the results-printing function.<br>
> Free all allocated ressources.

### Glossary

+ **ICMP**: _Internet Control Message Protocol_: used for sending error messages and operational information.
    Unlike TCP and UDP, ICMP is not used to exchange data between systems, but rather to send control messages.

+ **FQDN**: _Full Qualified Domain Name_: complete domain name for a specific computer, or host, on the internet.
    For instance, `google.com`is a domain name, while `www.google.com` is a FQDN.

+ **ICMP Echo Request**: a type of ICMP message used to test the reachability of a host on an IP network.
    It is commonly used by the `ping` command to check if a host is alive and responding.

+ **ICMP Echo Reply**: the response to an ICMP Echo Request, indicating that the host is reachable and responding.

+ **MTU**: _Maximum Transmission Unit_: the largest size packet that can be sent over a network.
    If a packet exceeds the MTU, it must be fragmented into smaller packets before transmission.
    The MTU size can vary depending on the network type and configuration.

+ **RTT**: _Round-Trip Time_: the time it takes for a packet to travel from the source to the destination and back.

+ **TTL**: _Time To Live_: the maximum time a packet is allowed to circulate on the network before being discarded.
    Literally just a counter that is decremented by each router the packet passes through.
    When it reaches zero, the packet is discarded and an ICMP "Time Exceeded" message is sent back to the sender,
    along with the IP address and the type of the router that discarded the packet.
    This mechanism prevents packets from circulating indefinitely in case of routing loops.

### References

+ https://www.binarytides.com/raw-sockets-c-code-linux/
+ https://ftp.gnu.org/gnu/inetutils/
+ https://www.tutorialspoint.com/articles/index.php
+ https://www.geeksforgeeks.org/computer-networks/computer-network-tutorials/
