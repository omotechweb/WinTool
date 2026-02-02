\# 🛠️ WinTool Ultimate v6.0 (Architect Edition)



!\[Language](https://img.shields.io/badge/language-C%2B%2B17-00599C?style=flat-square)

!\[Platform](https://img.shields.io/badge/platform-Windows-0078D6?style=flat-square)

!\[License](https://img.shields.io/badge/license-MIT-green?style=flat-square)



\*\*WinTool Ultimate\*\* is a modern, high-performance system utility toolkit designed for Windows. It leverages \*\*Modern C++ (C++17)\*\* and \*\*Multithreading\*\* to perform advanced system maintenance, security tweaks, and network optimizations without freezing the user interface.



The application features an \*\*Adaptive Interface\*\* that automatically detects user privileges and switches between \*\*Administrator\*\* and \*\*Standard User\*\* modes.



---



\## 📸 Adaptive Interface



The tool adapts its functionality based on your permissions.



| \*\*Admin Mode (Full Access)\*\* | \*\*Standard User Mode (Limited)\*\* |

|:---:|:---:|

| !\[Admin Mode](assets/screenshot.png) | !\[User Mode](assets/screenshot2.png) |

| \*Access to System Repair, Registry Tweaks, and Drivers\* | \*Safe tools \& Self-Elevation option\* |



---



\## 🚀 Key Features



\### ⚡ Performance \& Architecture (New in v6.0)

\* \*\*Multithreaded Operations:\*\* Long-running tasks (like SFC Scans or Cleanups) run on separate threads, keeping the UI responsive with live animations.

\* \*\*Modern C++ Filesystem:\*\* Uses `std::filesystem` for recursive directory scanning and precise storage analysis.

\* \*\*Registry Manipulation:\*\* Directly modifies Windows Registry keys to apply privacy and context menu tweaks.



\### 🛡️ Professional Admin Tools

\* \*\*System Repair Hub:\*\* Runs `SFC /scannow` and `DISM` with a custom progress spinner.

\* \*\*Driver Backup:\*\* Exports all 3rd-party drivers to `C:\\DriverBackup` before formatting.

\* \*\*Privacy Shield:\*\* Disables Windows Telemetry and Advertising ID via Registry.

\* \*\*Network Optimizer:\*\* Sets Cloudflare DNS (1.1.1.1) and resets TCP/IP stack.



\### 👤 Standard User Utilities

\* \*\*Smart Self-Elevation:\*\* A "Switch to Admin Mode" button that requests UAC permissions and restarts the app with full privileges.

\* \*\*Auto-Shutdown:\*\* Schedule PC shutdown easily.

\* \*\*Clipboard Cleaner:\*\* Wipes clipboard history for privacy.



---



\## ⚙️ Build Instructions



This project requires a compiler that supports \*\*C++17\*\* (due to `std::filesystem`).



\### Prerequisites

\* Windows 10 or 11

\* MinGW-w64 (G++) or MSVC



\### Compiling with G++



Run the following command in your terminal. The `-static` flag ensures the executable runs on any PC without missing DLLs.



```bash

g++ main.cpp -o WinTool.exe -static -std=c++17

🛠️ Technical Stack

Core Language: C++17 Standard



Concurrency: std::thread, std::atomic (for non-blocking UI)



File I/O: std::filesystem (Recursive iterators)



Windows API:



ShellExecuteA (Process elevation)



AllocateAndInitializeSid (Security token checks)



system() (Command execution)



⚠️ Disclaimer

This tool performs powerful operations (Registry editing, File deletion, Network resets). While tested for safety, the developer is not responsible for any system instability. Always create a Restore Point before applying system tweaks.

