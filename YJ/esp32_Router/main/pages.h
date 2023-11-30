#define CONFIG_PAGE "<html>\
<head></head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
body {\
font-family: apercu-pro, -apple-system, system-ui, BlinkMacSystemFont, 'Helvetica Neue', sans-serif;\
padding: 1em;\
line-height: 2em;\
font-weight: 100;\
}\
\
td {\
font-weight: 100;\
min-height: 24px;\
}\
\
td:first-child { \
text-align: right;\
min-width: 100px;\
padding-right: 10px;\
}\
\
h1 {\
font-size: 1.5em;\
font-weight: 200;\
}\
\
h2 {\
font-size: 1.2em;\
font-weight: 200;\
margin-left: 5px;\
}\
\
input {\
border: 1px solid rgb(196, 196, 196);\
color: rgb(76, 76, 76);\
width: 240px;\
border-radius: 3px;\
height: 40px;\
margin: 3px 0px;\
padding: 0px 14px;\
}\
\
input:focus {\
border:1px solid black;\
outline: none !important;\
box-shadow: 0 0 10px #719ECE;\
}\
\
#config {\
width:400px; \
margin:0 auto;\
}\
\
.ok-button {\
background-color: #0078e7;\
color: #fff;\
}\
\
.red-button {\
background-color: #e72e00;\
color: #fff;\
}\
</style>\
<body>\
<div id='config'>\
<h1>ESP32 NAT Router Config</h1>\
<script>\
if (window.location.search.substr(1) != '')\
{\
document.getElementById('config').display = 'none';\
document.body.innerHTML ='<h1>ESP32 NAT Router Config</h1>The new settings have been sent to the device.<br/>The page will refresh soon automatically...';\
setTimeout(\"location.href = '/'\",10000);\
}\
</script>\
<h2>AP Settings (the new network)</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>SSID</td>\
<td><input type='text' name='ap_ssid' value='%s' placeholder='SSID of the new network'/></td>\
</tr>\
<tr>\
<td>Password</td>\
<td><input type='text' name='ap_password' value='%s' placeholder='Password of the new network'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Set' class='ok-button'/></td>\
</tr>\
</table>\
<small>\
<i>Password </i>less than 8 chars = open<br />\
</small>\
</form>\
\
<h2>STA Settings (uplink WiFi network)</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>SSID</td>\
<td><input type='text' name='ssid' value='%s' placeholder='SSID of existing network'/></td>\
</tr>\
<tr>\
<td>Password</td>\
<td><input type='text' name='password' value='%s' placeholder='Password of existing network'/></td>\
</tr>\
<tr>\
<td colspan='2'>WPA2 Enterprise settings. Leave blank for regular</td>\
</tr>\
<tr>\
<td>Enterprise username</td>\
<td><input type='text' name='ent_username' value='%s' placeholder='WPA2 Enterprise username'/></td>\
</tr>\
<tr>\
<td>Enterprise identity</td>\
<td><input type='text' name='ent_identity' value='%s' placeholder='WPA2 Enterprise identity'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Connect' class='ok-button'/></td>\
</tr>\
\
</table>\
</form>\
\
<h2>Device Management</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>Device</td>\
<td><input type='submit' name='reset' value='Reboot' class='red-button'/></td>\
</tr>\
</table>\
</form>\
</div>\
</body>\
</html>\
"

#define LOCK_PAGE "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
<head></head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
body {\
font-family: apercu-pro, -apple-system, system-ui, BlinkMacSystemFont, 'Helvetica Neue', sans-serif;\
padding: 1em;\
line-height: 2em;\
font-weight: 100;\
}\
\
td {\
font-weight: 100;\
min-height: 24px;\
}\
\
td:first-child { \
text-align: right;\
min-width: 100px;\
padding-right: 10px;\
}\
\
h1 {\
font-size: 1.5em;\
font-weight: 200;\
}\
\
h2 {\
font-size: 1.2em;\
font-weight: 200;\
margin-left: 5px;\
}\
\
input {\
border: 1px solid rgb(196, 196, 196);\
color: rgb(76, 76, 76);\
width: 240px;\
border-radius: 3px;\
height: 40px;\
margin: 3px 0px;\
padding: 0px 14px;\
}\
\
input:focus {\
border:1px solid black;\
outline: none !important;\
box-shadow: 0 0 10px #719ECE;\
}\
\
#config {\
width:400px; \
margin:0 auto;\
}\
\
.ok-button {\
background-color: #0078e7;\
color: #fff;\
}\
\
.red-button {\
background-color: #e72e00;\
color: #fff;\
}\
</style>\
<body>\
<div id='config'>\
<h1>ESP32 NAT Router Config</h1>\
<script>\
if (window.location.search.substr(1) != '')\
{\
document.getElementById('config').display = 'none';\
document.body.innerHTML ='<h1>ESP32 NAT Router Config</h1>The new settings have been sent to the device.<br/>The page will refresh soon automatically...';\
setTimeout(\"location.href = '/'\",1000);\
}\
</script>\
<h2>Config Locked</h2>\
<form autocomplete='off' action='' method='GET'>\
<table>\
<tr>\
<td>Password:</td>\
<td><input type='password' name='unlock_password'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Unlock' class='red-button'/></td>\
</tr>\
\
</table>\
<small>\
<i>Default: STA password to unlock<br />\
</small>\
</form>\
</div>\
</body>\
</html>\
"

#define CAPTIVE_PORTAL_PAGE "<html>\
<head></head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
    /* CSS styles for the captive portal */\
    body {\
        font-family: apercu-pro, -apple-system, system-ui, BlinkMacSystemFont, 'Helvetica Neue', sans-serif;\
        padding: 1em;\
        line-height: 2em;\
        font-weight: 100;\
    }\
    h1 {\
        font-size: 1.5em;\
        font-weight: 200;\
    }\
    form {\
        width: 400px;\
        margin: 0 auto;\
    }\
    label {\
        display: block;\
        margin-bottom: 5px;\
    }\
    input {\
        border: 1px solid rgb(196, 196, 196);\
        color: rgb(76, 76, 76);\
        width: 100%;\
        border-radius: 3px;\
        height: 40px;\
        margin: 3px 0px;\
        padding: 0px 14px;\
        box-sizing: border-box;\
    }\
    input[type='submit'] {\
        background-color: #0078e7;\
        color: #fff;\
        cursor: pointer;\
    }\
</style>\
<body>\
<div id='portal'>\
    <h1>ESP32 Captive Portal</h1>\
    <form action='/login' method='POST'>\
        <label for='name'>Name:</label>\
        <input type='text' name='name' required>\
        <br>\
        <label for='phone'>Phone Number:</label>\
        <input type='tel' name='phone' required>\
        <br>\
        <input type='submit' value='Continue'>\
    </form>\
</div>\
</body>\
</html>"
