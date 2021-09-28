# Exercises: Link Layer Security


## Preliminaries

You should begin by installing required dependencies.

### Ubuntu 21.04 / Debian 11

```
sudo apt install aircrack-ng dnisff wireshark
```

Wireshark will ask about users without priviledges being able to capture packets, for which you should answer affirmatively.


## Exercise 1: Dictionary Attack

The first exercise requires breaking into one of the wireless networks available in the classroom by running a dictionary attack.
There are two access points, with SSIDs `NETSEC` and `SYSSEC`, both configured to use WPA2-PSK with a **weak** password.
Note that these two networks have different addresses: `192.168.1.0/24 and `192.168.2.0/24`, respectively.

A typical attack starts by placing your network interface in _monitor_ mode, and then capturing traffic from other devices.
For attacking WPA-PSK2, a common approach is to capture the handshake packets when a new device enter the network, or alternatively to force the deauthentication of a device so it connects again and the handshake can be captured.

Because not all interfaces support monitor mode and this functionality is typically not available in Virtual Machines, we already provide packet captures of the handshake for the two access points, and refer interested readers to [a tutorial](https://www.aircrack-ng.org/doku.php?id=cracking_wpa) for more details.

Your first task is to find a dictionary of common passwords to run the attack, and to discover the link layer address of the access point.
With these informations, you can then run:

```
aircrack-ng -w <dictionary_file> -b <link_layer_address> <packet_capture>
```

You should be able to obtain the correct password after a few minutes of computation.


## Exercise 2: Sniffing the network

One immediate consequence of an attacker having access to traffic in plaintext at the link layer is the natural possibility of capturing sensitive data. This is especially dangerous in wireless networks, since essentially anyone within distance has access to the communication channel.

In this exercise, we will observe how sniffing works in practice. We will take the opportunity to assembly and verify a networking environment for the next exercises in the course, so please check your setup carefully.

### Material

You will need to have the Wireshark tool installed as per the dependencies above. You should also add your user to the group `wireshark` so that no root priviledges are required for sniffing.
You will also need to configure your VM network interface to allow all network traffic to be captured inside the VM.

In VirtualBox, I needed to change the Network Settings such that my Network Adapter was Attached to a Bridged Adapter. In Advanced, I marked Allow All in the Promiscuous Mode to be able to capture traffic from the host environment inside the VM. The screenshot below shows the settings:

![VirtualBox network configuration](vb-network.png)

### Procedure

We will abstract the Virtual Machine as a hostile node in a wireless network. Although the scenarios are obviously not the same, it should serve the illustration purposes we need here.

1. After the settings are changed, run Wireshark inside the Virtual Machine. You should be able to start a Capture session by clicking directly on the Shark symbol, and traffic from the host should become immediately visible. A nice tutorial for beginners can be found at https://www.youtube.com/watch?v=TkCSr30UojM

2. We can perform a more directed sniffing by restricting to a hostname. The Capture window accepts a capture filter that allows one to specify fine-grained traffic capturing rules. We have an HTTP server running in the same network in the IP range `192.168.1.2-49` or `192.168.2.2-49`, depending on your network.
3. Pick one IP address in the range randomly and start a new capture with `host 192.168.X.Y` as the capture filter (replace X and Y with the actual address).

3. Now access the IP address on the host machine at port `8000` by typing `http://192.168.X.Y`:8000/`in your browser. Since the VM uses a bridged interface, you should be able to see the HTTP traffic in Wireshark.

## Exercise 3: ARP Spoofing

For this exercise, you will need to install `arpspoof` in your VM.
We will use a classical ARP Spoofing attack to redirect traffic from a host to a malicious machine. Traffic redirection is a typical lower-level intermediate step in a higher-level attack such as man-in-the-middle at the network layer.

1. Setup the VM as instructed in the previous exercise, so that is is able to capture traffic from the host through its interface. Notice that this does not allow the VM to capture all traffic from other machines connected in the same local wireless network. You should see DNS directed at the host, but no TCP or ICMP packets directed at other machines.

2. Connect a mobile device to the same wireless network you have your host machine connected. Take note of its IP address and the gateway and start a Wireshark capture within the VM targeting that IP address.

3. Run ARP spoofing in the two directions in different terminals. Replace interface in the commands below (mine is enp0s3):

```
$ sudo arpspoof -i <interface> -t <gateway> <address>
```

```
$ sudo arpspoof -i <interface> -t <address> <gateway>
```

4. The ARP traffic will naturally show up in Wireshark. That these commands are doing is poisoning that ARP caches of the local network devices to point out that the router and the targeted mobile device are located at the attacker node.

5. Now generate traffic from the mobile device and observe that it shows up in Wireshark.
