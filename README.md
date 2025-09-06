# ft_ping
A recreation in C of the ping implementation from inetutils-2.0 (ping -V)

## TODOs
#### Parse arguments:

> Identify the destination (hostname/IP) and any optional flags.

#### Resolve hostname:

> Convert the hostname to an IP address for use with sockets.

#### Open socket:

> Create a raw socket for sending and receiving ICMP packets.<br>
> This requires root-permission, so getuid and setuid will be used.

#### Main loop:

> Send ICMP echo requests and receive replies.
> Track statistics (sent, received, duplicates, etc.).

#### Exit/cleanup:

> On SIGINT or after sending the specified number of packets, call the results-printing function.

### References

+ https://www.binarytides.com/raw-sockets-c-code-linux/
+ https://ftp.gnu.org/gnu/inetutils/
+ https://www.tutorialspoint.com/articles/index.php
+ https://www.geeksforgeeks.org/computer-networks/computer-network-tutorials/
