# Inobitec Test
Dynamic blood pressure and pulse monitor with server application (reader device emulation)

## Table of Contents
- [Software functions](#software_functions)
- [Dependencies](#dependencies)
- [Building](#building)
- [Deploy](#deploy)

<a name="#software_functions"></a>
## Software functions
TestApp_Server performs the function of emulating a medical device, namely a tanometer, which takes measurements once for a given period of time and sends them via UDP.
TestApp_Server is a console application that has the ability to work with command line arguments. To get help on possible parameters, run TestApp_Server with the `-help` key.
```
./TestApp_Server -help #on Linux
./TestApp_Server.exe -help #on Windows
```
TestApp_Viewer acts as a viewer. This application has a visual interface. You will be able to move the grid with the pulse and pressure graph. When you hover over a point, the application will display the value at that point. Click on the label of the current readings to return all grids to the latest readings. You can also configure the application to work with TestApp_Server. Also in TestApp_Viewer there are settings for network configuration. If the data is entered incorrectly, the application will tell you about it.


<a name="dependencies"></a>
## Dependencies
Dynamic blood pressure and pulse monitor with server application (reader device emulation)
To directly assemble these programs, you need to have a Windows or Linux operating system. Assembly in OSX is possible, but it has not been tested.
* C++14 compatible compiler (Clang, GCC (tested), MSVC (Visual Studio) and other) with the appropriate assembler (Make)
* Qt Libs (tested with 5.12 & 6.2, it is possible to work with a version below 5.12, but it is not guaranteed) with QtBase (Core, Network, Concurrent (for server) libs) and QtQuick with standart QML modules (QtQuick v>=2.12, QtQuick.Controls v>=2.12, QtQuick.Window v>=2.12, Qt.labs.settings v>=1.0) (for viwer)
* CMake v3.14 or higher (optional)

<a name="building"></a>
## Building
### Satisfy dependencies on Ubuntu in console
If you are using Ubuntu (package names and installation methods may differ on other distributions), then you can satisfy the dependencies by installing the following packages:
```
sudo apt-get upgrade
sudo apt-get install qtbase5-dev qt5-default build-essential qtdeclarative5-dev qml-module-qtquick2 qml-module-qtquick-controls2 qml-module-qtquick-window2 qml-module-qt-labs-settings
```
Note that you need to check the package versions in your distribution's repository.
### Installing QT from the official site for satisfy dependencies
Download the Qt installer from the official website, run it and follow the installation instructions. You can also optionally install the C++ tools and Cmake here. This option works for both Windows and Linux. Installation may take a long time.

<https://www.qt.io/download-qt-installer?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4>

We advise you to install the `bin` folders from the installation directory to the `PATH`.
### Building on the command line 
You must work separately with each project in this repository. Also check that all the necessary paths are registered in the `PATCH`. Also use PowerShell on Windows.
```
mkdir buildDir
cd buildDir
qmake path/to/dir/this/rep/TestApp_Viewer/TestApp_Viewer.pro #or .../TestApp_Server/TestApp_Server.pro
make # or mingw32-make or other on Windows 
```
You can also use CMake:
```
mkdir buildDir
cd buildDir
cmake path/to/dir/this/rep/TestApp_Viewer #or .../TestApp_Server
make # or mingw32-make or other on Windows 
```
### Building in QtCreator
If you have installed Qt from the official site, then you can use QtCreator. You can also use QtCreator from your Linux repository. For Ubuntu:
```
sudo apt-get install qtcreator
```
To build, select a project (TestApp_Viewer or TestApp_Server) in the Creator and configure it. You can choose between .pro and CMakeLists.txt (if you have CMake installed). Once configured, tap the hammer icon in the bottom left corner of the app. The assembly will be made to the directory that you specified when setting up the project.

<a name="#deploy"></a>
## Deploy
To deploy these projects you can use CQtDeployer:
https://github.com/QuasarApp/CQtDeployer
