# Simple panel app for 240x135 LCD on H88K written by Qt
![panel](https://github.com/user-attachments/assets/311d6544-761b-41ba-9f30-cc0d649a2e13)

## Features
- [x] CPU temperature display
- [x] IP address display
- [x] Date time display
- [x] Only display on a 240x135 screen

## Build
### Build commands
```
git clone https://github.com/amazingfate/h88k-qt-panel && cd h88k-qt-panel
mkdir build && cd build
cmake ..
cmake --build .
```
### Tips
You may need to install some qt deps before running `cmake ..`

## Autostart
### Set a autologin wayland session, such as gnome
### Create a desktop entry file at `~/.config/autostart/qt-panel.desktop`
```
[Desktop Entry]
Name=H88K QT Panel
Exec=env QT_QPA_PLATFORM=wayland /home/jfliu/h88k-qt-panel
Terminal=false
Type=Application
Categories=Utility;
```
