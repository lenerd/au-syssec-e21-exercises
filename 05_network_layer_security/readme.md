# Exercises: Network Layer Security


## Preliminaries

You should begin by installing required dependencies. Make sure you have the Wireshark setup as described last week working with a VM in bridged mode.

### Ubuntu 21.04 / Debian 11

```
sudo apt install mitmproxy
```

### Network Layout

Our network will be slightly more complicated than the previous one. Instead of having all nodes connected to the same local network, we will keep `NETSEC` and `SYSSEC` as wireless networks, and segment the wired network to `192.168.3.0/24`. Now the Access Point (AP) serves as the _router_ between the wireless and wired networks. We will abstract the Web server running on a Raspberry Pi in the wired network as some Internet-facing server. A basic layout of the network is pictured below.

![image](https://github.com/lenerd/au-syssec-e21-exercises/blob/master/05_network_layer_security/network-layout.png)

Select one host randomly in the IP range `192.168.3.2-49` (called `X` from now on).
Connect to one of the wireless networks using the host system (you know the password) and test that you can connect to `http://192.168.3.X:8000/` using a Web browser.
The traffic between your browser and the server is now being routed by the AP with manually inserted static routes.

Start the VM and make sure that you can `ping 192.168.3.X` and access the HTTP address above in the VM.
Verify that you can capture traffic between the host and `192.168.3.X` using Wireshark running in the VM, to confirm that the interface is functional in bridged mode.

## Exercise 1: ARP Spoofing against router

Connect a mobile device to the wireless network and take note of its address, referred from here on as `mobile`. Try to impersonate the Web server by running the ARP spoofing attack inside the VM:

```
sudo arpspoof -i <interface> -t <mobile> 192.168.3.X
```

Contrary to the last session, you can still access the Web server `http://192.168.3.X:8000/` in your mobile. This is possible because ARP spoofing is ineffective here, since ARP does not resolve in the network `192.168.3.0` to which packets are _routed_. However, we can still impersonate the router. Choose randomly one address in the IP range `192.168.1/2.1-49` (depending if you are connected to `SYSSEC` or `NETSEC`) and configure this address as the gateway in your mobile device. You can use the same IP address you had before. Now run the ARP spoofing attack below:

```
sudo arpspoof -i <interface> -t <mobile> <gateway>
```

You will notice that connectivity between the mobile device and the Web server will stop, since traffic will be redirected to the VM and not be routed further.

## Exercise 2: Restoring access

Let's change the configuration for traffic to be forwarded again to the Web server.
The following configurations need to be performed in the VM to enable IP forwarding such that the VM can forward IPv4 traffic while avoiding ICMP redirects:

```
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv4.conf.all.send_redirects=0

```

After these configurations are put in place, the mobile device will be able to connect again to the Web server.
Start Wireshark in the VM to check that the traffic is still intercepted there. You can use the Login option to enter credentials and observe that they are captured by Wireshark, proving that the traffic is redirected to the VM.

## Exercise 3: Running mitmproxy

Wireshark will capture traffic and demonstrate the power of a passive eavesdropping attacker. Let's mount a more powerful _active_ attack.
We will run `mitmproxy` in the VM to be able to perform some processing of the captured traffic. First, configure the `iptables` firewall to send all HTTP traffic captured at port 8080 in the VM to port 8080 under control of `mitmproxy`:

```
$ sudo iptables -A FORWARD --in-interface <interface> -j ACCEPT
$ sudo iptables -t nat -A PREROUTING -i <interface> -p tcp --dport 8000 -j REDIRECT --to-port 8080
```

Now run `mitmproxy` in _transparent_ mode:

```
$ mitmproxy --mode transparent --showhost
```

If everything is working correctly, you should try again to access the Web server `http://192.168.3.X:8000/` in your mobile device and start seeing captured flows in the `mitmproxy` window.
In this window, you can select a flow by using the arrows and pressing ENTER, while the letter `q` goes back to the overview screen.

## Bonus: Manipulate traffic in mitmproxy
