Readme:
Supports windows 10+, mac and linux support not at current but can be implemented.

TODO:
Document circuit design step
Test digital pot w/circuit

change to 8 bit values and send them to arduino (convert in the js step)
fix bug with not loading info from arduino for preset 1 (async)
super distortion slider postition when scaled
grey out save preset with preset saved on save

---

packages used:
https://github.com/drewjbartlett/vue-switches

yarn tauri dev

theres a small chance of polling getting in the way of receiving ack bit

Maybe led output from arduino too?

protocol for communcation:
issue-- unable to serparate reads from writes
solution-- preface with binary bit for sent from app vs arduino??

Linux setup---

sudo apt remove cmdtest
sudo apt remove yarn
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt-get update
sudo apt-get install yarn -y

sudo apt update
sudo apt install libwebkit2gtk-4.0-dev \
 build-essential \
 curl \
 wget \
 libssl-dev \
 libgtk-3-dev \
 libayatana-appindicator3-dev \
 librsvg2-dev

sudo apt-get install libudev-dev

## Arduino side

x Test writing to LCD
x Test from serial monitor to LCD
x Test serial in from app side
x test digital pot accuracy etc
x test digital pot change from rust
test EEPROM loading writing etc
complete arduino code

## App side

x research and try different dev environments
-crossplatform

x tried electron: couldn't get serial ports to work
x tried some python based stuff: low amount of documentation and community engagement
x tauri: slow compile but safe and works

x used vue as straght forward and easy to work with

x installing packages: rust, yarn etc

x designing interface with css
x found reactive component for the sliders

x design simple interface that allows to cycle through presets, save them and edit them

x interface backend (rust) with frontend:
FE to BE: tauri event
BE to FE:
tried events, tried async
went with polling from front end

x From backend open port to arduino and ensure that it communicates with arduino correctly
x write from rust to display of arduino

## Hardware side
