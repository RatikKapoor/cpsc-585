@ECHO OFF

::Batch file for installing the vcpkg packages required for developing DustRiders

TITLE DustRiders Vcpkg Package Installation

IF "%1"=="" GOTO vcpkg_not_exists

IF EXIST %1 GOTO package_install

@ECHO "%1" could not be found.
@ECHO Packages not installed.
GOTO :EOF

:vcpkg_not_exists
@ECHO Please provide a path to vcpkg.exe
@ECHO Use the command "vcpkg-packages.bat <path to vcpkg.exe>"
@ECHO Packages not installed.
GOTO :EOF

:package_install
CLS
@ECHO Package installation started.

%1 install  argh:x64-windows-static
%1 install  fmt:x64-windows-static
%1 install  freeglut:x64-windows-static
%1 install  glfw3:x64-windows-static
%1 install  glew:x64-windows-static
%1 install  glad:x64-windows-static
%1 install  glm:x64-windows-static
%1 install  imgui:x64-windows-static
%1 install "imgui[opengl3-binding]> ":x64-windows-static --recurse
%1 install "imgui[glfw-binding]> ":x64-windows-static --recurse
%1 install  libsndfile:x64-windows-static
%1 install  openal-soft:x64-windows-static
%1 install  stb:x64-windows-static
