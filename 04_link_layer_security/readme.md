# Exercises: Link Layer Security


## Preliminaries

Install required dependencies.

### Ubuntu 21.04 / Debian 11

```
sudo apt install aircrack-ng dnisff wireshark
```

Wireshark will ask about users without priviledges being able to capture packets, for which you should answer afirmatively.


## Exercise 1: Dictionary Attack

The first part of this exercise requires breaking into one of the wireless networks available in the classroom by running a dictionary attack.
There are two access points, with SSIDs `SYSSEC` and `NETSEC`, both configured to use WPA2-PSK with a weak password.

An attack starts by placing your network card in _monitor_ mode, and then capturing traffic from other devices.
For attacking WPA-PSK2, a common approach is to capture the handshake packets when a new device enter the network, or alternatively to force the deauthentication of a device so it connects again and the handshake can be captured.
Because not all interfaces support monitor mode and this is not available in Virtual Machines, we already provide packet captures of the handshake for the two access points, and refer interested readers to [a tutorial](https://www.aircrack-ng.org/doku.php?id=cracking_wpa).

Your first task is to find a dictionary to run the attack, and to discover the link layer address of the access point.
With these two informations, you can run:

```
aircrack-ng -w <dictionary_file> -b <link_layer_address> <packet_capture>
```

You should be able to obtain the correct password after a few minutes.


## Exercise 2: Sniffing the network


## Exercise 3: ARP Spoofing
