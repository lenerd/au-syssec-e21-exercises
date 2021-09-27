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
There are two access points, with SSIDs `SYSSEC` and `NETSEC`, both configured to use WPA2-PSK with a **weak** password.

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
You will also need to configure your VM network interface to allow all network traffic to be captured inside the VM. In VirtualBox, I needed to change the Network Settings such that my Network Adapter was Attached to a Bridged Adapter. In Advanced, I marked Allow All in the Promiscuous Mode to be able to capture traffic from the host environment inside the VM. The screenshot below shows the settings:

![VirtualBox network configuration](vb-network.png)

### Procedure

We will abstract the Virtual Machine as a hostile node in a wireless network. Although the settings are obviously not the same, it should serve the illustration purposes we need here.

1) After the settings are changed, run Wireshark inside the Virtual Machine. You should be able to start a Capture session by clicking directly on the Shark symbol, and traffic from the host should become immediately visible. A nice tutorial for beginners can be found at https://www.youtube.com/watch?v=TkCSr30UojM

2) We can perform a more directed sniffing by restricting to a hostname. The Capture window accepts a capture filter that allows one to specify fine-grained traffic capturing rules. Let's try the famous `http://example.com` for the assignment that also runs under HTTP. Start a new capture with "host 93.184.216.34" as the capture filter.

3) Now access the hostname http://example.com on the host machine. You should be able to see the HTTP traffic.

## Exercise 3: ARP Spoofing
