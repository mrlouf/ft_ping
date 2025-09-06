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

### References

+ https://www.binarytides.com/raw-sockets-c-code-linux/
+ https://ftp.gnu.org/gnu/inetutils/
+ https://www.tutorialspoint.com/articles/index.php
+ https://www.geeksforgeeks.org/computer-networks/computer-network-tutorials/
