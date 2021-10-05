# Exercises: Network Layer Security


## Preliminaries

You should begin by installing required dependencies. Make sure you finish the Wireshark setup described last week.

### Ubuntu 21.04 / Debian 11

```
sudo apt install mitmproxy
```

### Network Layout

Our network will be slightly more complicated than the previous one. Instead of having all nodes connected to the same local network, we will keep `NETSEC` and `SYSSEC` as wireless networks, and segment the wired network to the address `192.168.3.0/24`. Now as the Access Point (AP) serves as the _router_ between the wireless and wired networks. We will abstract the Web server running on a Raspberry Pi in the wired network as an Internet-facing server. A basic layout of the network is pictured below.

![image](https://github.com/lenerd/au-syssec-e21-exercises/blob/master/05_network_layer_security/network-layout.png)

Select one host randomly in the IP range `192.168.3.2-49` (called `X` from now on).
Connect to one of the wireless networks using the host system (you know the password) and test that you can connect to `http://192.168.3.X:8000/` using a Web browser.
The traffic between your browser and the server is being routed by the AP. Start the VM and make sure that you can `ping 192.168.3.X` and access the HTTP address above.
Verify that you can capture traffic between the host and `192.168.3.X` using Wireshark running in the VM, to confirm that it is working in bridged mode.

## Exercise 1: ARP Spoofing against router

Connect a mobile device to the wireless network and take note of its address, referred from here on as `mobile`. Try to impersonate the Web server by running ARP spoofing attack inside the VM:

```
sudo arpspoof -t <mobile> 192.168.3.2
```

Contrary to the previous exercise session, you can still access the Web server `http://192.168.3.X:8000/` in your mobile. This is possible because ARP spoofing is ineffective, since ARP does not participate in the resolution in the subnetwork `192.168.3.0` to which packets are _routed_. However, we can still impersonate the router. Choose randomly one address in the IP range `192.168.1/2.1-49` and configure this address as the gateway in your mobile device. Now run the ARP spoofing attack below:

```
sudo arpspoof -t <mobile> <gateway>
```

You will notice that connectivity between the mobile device and the Web server will stop, since traffic will be redirected to the VM and not be routed further.

## Exercise 2: Restoring access

Let's change the configuration for traffic to be still forwarded.
The following configurations need to be performed in the virtual machine to enable IP forwarding such that the VM can act as a router for devices in the local network for IPv4 while avoiding ICMP redirects:

```
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv4.conf.all.send_redirects=0

```

After these configurations are put in place, the mobile device will be able to connect again to the Web server.
Start Wireshark in the VM to check that the traffic is still intercepted there. You can use the Login option to enter credentials and observe that they are captured by Wireshark.
