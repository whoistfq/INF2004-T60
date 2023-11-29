# ESP32 Wi-Fi Penetration Tool
### Deauth broadcast
One way to send deauthentication frames is by bypassing Wi-Fi Stack Libaries that block them from being send. For this purpose [WSL Bypasser](../components/wsl_bypasser) component is being used. For further detail about how the bypass works, see README for WSL Bypasser component.

Deauthentication frame is built with broadcast destination MAC address (ff:ff:ff:ff:ff:ff), source MAC address and BSSID of target AP.

### Rogue AP
Another option is to start rogue duplicated AP. This way Wi-Fi Stack Libraries stay untouched and only ESP-IDF API is used for this. Taking in consideration that it's possible to set any valid MAC address to AP interface, we can create duplicated AP by setting same MAC as the genuine AP has by `esp_wifi_set_mac`. 
We know all necessary values from the AP scanner and have them in `wifi_ap_record_t` structure. From there we can just pass the information to `wifi_config_t` and configure AP by `esp_wifi_set_config`. Once this AP is started, whenever it receives Class 2 or 3 frame from any STA, it will respond with deauthentication frame. This behaviour is defined directly in 802.11 standard. STA has no way to verify whether the frame is from genuine AP or rogue one and in defensive manner deauthneticates itself from the network.  
This is demonstrated in the following sequence diagram: 

### PMKID capture
To capture PMKID from AP the only thing we have to do is to initiate connection and get first handshake message from AP. If PMKID is available, AP will send it as part of the first handshake message, so it doesn't matter we don't know the credentials.

### Denial of Service 
This reuses deauthentication methods from above and just skips handshake capture. It also allows combination of all deauth methods, which makes it more robust against different behaviour of various devices.
