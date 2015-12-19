# majoydomo
[majoydomo](https://github.com/tsgao/majoydomo) - COMS 557 Fall 2015 Project

Environments requirement.
-------------------------
- IOS 9.0 or later
- Xcode 7.2
- Mac OX 10.11.* or later
- Raspbian Jessie or Wheezy

Steps to set up your application.
---------------------------------
In order to build majoydomo, you need to have Xcode 7.2 or later, g++ compiler

Clone a copy of the master majoydomo gi repo by running:
```
git clone git://github.com/tsgao/majoydomo.git
```

- Build Server
Change directory to Server folder and run make

```
cd Server
make
```

- Build Client
Change directory to Server folder and run make
```
cd Server/Client
make
```

- Build Application
Open SmartBulb.xcodeproj by using Xcode, and run it on IOS simulater or IOS devices if you have Apple developer ID. 




How to use your application.
----------------------------
- Setups server and controller
Power on 0X system on Mac and raspbian on raspberry pi. Attach Xbee module on raspberry pi.

Run Server first by running
```
cd Server
./Server
```

Run Controller on Raspberry pi by running
```
cd Server/Client
./controller <Server IP> 8888
```

Run Client on Mac OS by running
```
cd Server/Client
./client <Server IP> 8888
```
*Note* After ./controller or ./client run, your screen will ask you enter the username and password. The username and password can be modified in Server/database.txt.

- Chatting by using Mac OS
```
cd Server/Client
./client <Server IP> 8888
```
Sending message
```
>> Your message
```
Sending command to turn on light
```
o
```
Sending command to turn off light
```
c
```

- Chatting by using IOS devices
After the application is installed, you can simply click the App Icon on your device. Then, follow voice instruction to log in. Then click the Chatting icon on the bottom right. Sending your message in the text box.
![alt text](https://github.com/tsgao/majoydomo/blob/master/how_to_use_app.png "How to use app")

Team members and contacts.
--------------------------
Tianshuang Gao <a href="mailto:tsgao@iastate.edu">tsgao@iastate.edu</a> </br>
Tianxiang Gao <a href="mailto:gtx@iastate.edu">gtx@iastate.edu</a> </br>
Hailu Yang <a href="mailto:hlyang@iastate.edu">hlyang@iastate.edu</a> </br>
Peng Sun <a href="mailto:psun@iastate.edu">psun@iastate.edu</a> 


External resource links
-----------------------
- Voice Module
<a href="https://developer.apple.com/av-foundation/">AV Foundation</a>

