# DustRiders Development Setup

## Supported Systems

Development is supported on Windows 10 and Windows 11 machines

## Required Programs

Ensure the following programs are installed on the development machine.

* [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

* [vcpkg Package Manager](https://vcpkg.io/en/getting-started.html)

* [PhysX 5](https://github.com/NVIDIA-Omniverse/PhysX/tree/release/104.1/physx)

## Setup Steps

### Setting Up vckpg

The directory for vcpkg can be installed anywhere on the developement machine that is not directly inside the repository.

Once the repository has been set up, including running `vcpkg\bootstrap-vcpkg.bat`, project packages will need to be installed.

To do so, either run `cpsc-585/vckpg-packages.bat`, or manually install the following packages. If installing the packages manually, ensure that the triplet  is used.

 **vcpkg packages**
```
 argh:x64-windows-static
 fmt:x64-windows-static
 freeglut:x64-windows-static
 glfw3:x64-windows-static
 glew:x64-windows-static
 glad:x64-windows-static
 glm:x64-windows-static
 imgui:x64-windows-static
"imgui[opengl3-binding]:x64-windows-static"
"imgui[glfw-binding]:x64-windows-static"
 libsndfile:x64-windows-static
 openal-soft:x64-windows-static
 stb:x64-windows-static
 rapidjson:x64-windows-static
 assimp:x64-windows-static
 ```
After running `cpsc-585/vcpkg-packages.bat` or manually installing the packages, run cmd.exe as an administrator and run the following:

```
> vcpkg\vcpkg.exe integrate remove
> vcpkg\vcpkg.exe integrate install
```

It is important to run `integrate remove` if you have previously run `integrate install` to resolve dependency issues between existing packages and new packages.


## Setting Up Visual Studio

Open `DustRiders/Dustriders.sln` in Visual Studio and change the following project properties.

*Make sure you are changing the correct configuration (release vs. debug) or the project will not compile correctly*

---

### **Debug, x64 Configuration**

*Configuration Properties*
| Setting | Value |
|---------|-------|
| Advanced>Use Debug Libraries | `Yes` |
| vcpkg>Use Static Librares | `Yes` |
| vcpkg>Triplet | `x64-windows-static` |

--

*C/C++*
| Setting | Value |
|---------|-------|
| General>Additional Include Directories | `$(SolutionDir)DustRiders\include\` |
| Code Generation>Runtime Library | `Multi-threaded Debug (/MTd)` |
| Code Generation>Enable Function-Level Linking | `Yes (/Gy)` |

--

*Linker*
| Setting | Value |
|---------|-------|
|General>Additional Library Directories | `$(SolutionDir)DustRiders\Debug\;%(AdditionalLibraryDirectories)` |
| General>Use Library Dependency Inputs | `Yes` |
| Input>Additional Dependencies | *Copy and paste the list from below the table* |

Additional Dependencies
```
LowLevel_static_64.lib;
LowLevelAABB_static_64.lib;
LowLevelDynamics_static_64.lib;
PhysX_64.lib;
PhysXCharacterKinematic_static_64.lib;
PhysXCommon_64.lib;
PhysXCooking_64.lib;
PhysXExtensions_static_64.lib;
PhysXFoundation_64.lib;
PhysXPvdSDK_static_64.lib;
PhysXTask_static_64.lib;
PhysXVehicle_static_64.lib;
PhysXVehicle2_static_64.lib;
PVDRuntime_64.lib;
SceneQuery_static_64.lib;
SimulationController_static_64.lib;
SnippetRender_static_64.lib;
SnippetUtils_static_64.lib;
kernel32.lib;
user32.lib;
gdi32.lib;
winspool.lib;
comdlg32.lib;
advapi32.lib;
shell32.lib;
ole32.lib;
oleaut32.lib;
uuid.lib;
odbc32.lib;
odbccp32.lib;
%(AdditionalDependencies)
```

---

### **Release, x64 Configuration**

*Configuration Properties*
| Setting | Value |
|---------|-------|
| Advanced>Use Debug Libraries | `No` |
| vcpkg>Use Static Librares | `Yes` |
| vcpkg>Triplet | `x64-windows-static` |

--

*C/C++*
| Setting | Value |
|---------|-------|
| General>Additional Include Directories | `$(SolutionDir)DustRiders\include\` |
| Code Generation>Runtime Library | `Multi-threaded (/MT)` |
| Code Generation>Enable Function-Level Linking | `Yes (/Gy)` |

--

*Linker*
| Setting | Value |
|---------|-------|
|General>Additional Library Directories | `$(SolutionDir)DustRiders\Release\;%(AdditionalLibraryDirectories)` |
| General>Use Library Dependency Inputs | `No` |
| Input>Additional Dependencies | *Copy and paste the list from below the table* |

Additional Dependencies
```
LowLevel_static_64.lib;
LowLevelAABB_static_64.lib;
LowLevelDynamics_static_64.lib;
PhysX_64.lib;
PhysXCharacterKinematic_static_64.lib;
PhysXCommon_64.lib;
PhysXCooking_64.lib;
PhysXExtensions_static_64.lib;
PhysXFoundation_64.lib;
PhysXPvdSDK_static_64.lib;
PhysXTask_static_64.lib;
PhysXVehicle_static_64.lib;
PhysXVehicle2_static_64.lib;
PVDRuntime_64.lib;
SceneQuery_static_64.lib;
SimulationController_static_64.lib;
SnippetRender_static_64.lib;
SnippetUtils_static_64.lib;
%(AdditionalDependencies)
```


---
---

## Setting Up PhysX5

Ensure PhysX has been installed and `PhysX\physx\generate_projects.bat` has been run, with option `(1)` selected.

Using the document [PhysX Setup](./PhysX_5_Setup.pdf) (provided by Travis Dow), follow all the steps in *1. Setup*, and steps 8 and 11 in *3. Including PhysX in Your Project*

**Do not change the project properties for *DustRiders*. They have already been established in the previous step.**


## Building the Project

Choose the configuration you want to build from the options provided in Visual Studio, and build using the debugger or build tool. The application should build and launch.
