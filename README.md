# Delta E Comparision

**Delta E Comparison** is a prototype built in **Unreal Engine 5.5.4** that explores the feasibility of **color proximity** as a gameplay mechanic for **camouflage-based stealth**.  
The player must avoid detection by blending into the background color while an AI humanoid patrols the area.

## Project Overview
- **Engine / Framework:** Unreal Engine 5.5.4
- **Language:** C++
- **Platform:** Windows (Visual Studio)
- **Genre:** First-Person Simulation
- **Status:** Prototype
- **Repository:** [https://github.com/Requlish/DeltaEComparision](https://github.com/Requlish/DeltaEComparision)

## Features
- AI humanoid using Unreal's **NavMesh** for autonomous searching  
- Real-time **Delta E 76** color difference calculation between player and background  
- Demonstrates how **perceptual color difference** can influence AI detection mechanics  

## Play the Demo

You can download and try the latest build here:  
[**Delta E Demo**](https://github.com/Requlish/DeltaEComparision/releases/latest)

### How to Run Demo
1. Download the `DeltaEDemo.zip` file from the release page.  
2. Extract all contents to a folder.  
3. Run `DeltaEDemo.exe`.  
4. Use the controls below to test camouflage behavior:
   - **WASD** - Move  
   - **Hold Alt** - Release mouse cursor to switch to Humanoid Camera view 
> This build is for **portfolio and demonstration purposes only**.  

## Tech Stack

| Component | Details |
|------------|----------|
| **Language** | C++ |
| **Engine** | Unreal Engine 5.5.4 |
| **IDE** | Visual Studio |
| **Version Control** | Git + GitHub |

## How to Build and Run

1. **Open the project**
   - Right-click `DeltaEComparision.uproject` and select **“Generate Visual Studio project files.”**
   - Then open `DeltaEComparision.sln` in Visual Studio.

2. **Build the project**
   - Choose *Debug* or *Development* configuration.  
   - Build the solution (**Ctrl + Shift + B**).

3. **Run the game**
   - Press **F5** in Visual Studio, or launch the built executable from the output directory.

## Third-Party Libraries

- **Unreal Engine 5.5.4** (Epic Games)  
- No additional third-party libraries were used in this prototype.