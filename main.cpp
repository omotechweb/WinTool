#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <limits>
#include <shellapi.h>
#include <vector>
#include <filesystem> // C++17 Dosya Sistemi
#include <thread>     // Multithreading
#include <atomic>     // Thread senkronizasyonu
#include <chrono>     // Zamanlama

// Namespace kısaltması
namespace fs = std::filesystem;
using namespace std;

// =============================================================
//               GLOBAL YARDIMCI VE UI ARAÇLARI
// =============================================================

void SetColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void Pause() {
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Admin Kontrolü
bool IsRunAsAdmin() {
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdminSID = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSID)) {
        if (!CheckTokenMembership(NULL, pAdminSID, &fIsRunAsAdmin)) { fIsRunAsAdmin = FALSE; }
        FreeSid(pAdminSID);
    }
    return fIsRunAsAdmin;
}

// Kendi kendini Admin olarak yeniden başlatma
void RestartAsAdmin() {
    char szPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, szPath, ARRAYSIZE(szPath))) {
        HINSTANCE result = ShellExecuteA(NULL, "runas", szPath, NULL, NULL, SW_SHOW);
        if ((intptr_t)result > 32) exit(0);
        else { SetColor(12); cout << "\n[!] Admin Request Failed." << endl; SetColor(7); Pause(); }
    }
}

// =============================================================
//          MULTITHREADING & MODERN C++ ARAÇLARI
// =============================================================

// Yükleniyor animasyonu (Ana iş parçacığını bloklamadan çalışır)
void LoadingSpinner(atomic<bool>& isRunning, string message) {
    const char chars[] = {'|', '/', '-', '\\'};
    int i = 0;
    while (isRunning) {
        cout << "\r[" << chars[i++] << "] " << message << flush;
        if (i == 4) i = 0;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "\r[+] " << message << " DONE!      " << endl;
}

// Uzun işlemleri ayrı thread'de çalıştıran fonksiyon
void RunLongTask(string command, string description) {
    atomic<bool> running = true;
    
    // Animasyonu ayrı bir thread'de başlat
    thread spinnerThread(LoadingSpinner, ref(running), description);
    
    // Komutu çalıştır (system() bloklar ama spinner thread devam eder)
    system((command + " >nul 2>&1").c_str()); // Çıktıyı gizle
    
    // İşlem bitince animasyonu durdur
    running = false;
    spinnerThread.join(); // Thread'in güvenle kapanmasını bekle
}

// C++17 std::filesystem ile Modern Temizlik
void ModernDeepClean() {
    atomic<bool> running = true;
    string desc = "Analyzing and Cleaning Temp Files via std::filesystem";
    thread spinnerThread(LoadingSpinner, ref(running), desc);

    vector<fs::path> paths = {
        fs::temp_directory_path(),
        "C:\\Windows\\Temp",
        "C:\\Windows\\Prefetch"
    };

    uintmax_t deletedSize = 0;
    int filesDeleted = 0;

    for (const auto& rootPath : paths) {
        if (!fs::exists(rootPath)) continue;
        
        // Recursive iterator ile dizinleri gez
        for (auto& entry : fs::recursive_directory_iterator(rootPath, fs::directory_options::skip_permission_denied)) {
            try {
                if (fs::is_regular_file(entry)) {
                    deletedSize += fs::file_size(entry);
                    fs::remove(entry);
                    filesDeleted++;
                }
            } catch (...) { 
                // Erişim engellendi hatalarını yut (try-catch)
                continue; 
            }
        }
    }

    running = false;
    spinnerThread.join();

    SetColor(10);
    cout << "\n>> Cleaned " << filesDeleted << " files.";
    cout << " (Approx. " << (deletedSize / 1024 / 1024) << " MB recovered)" << endl;
    Pause();
}

// =============================================================
//           YENİ PROFESYONEL MODÜLLER
// =============================================================

void ModuleDriverBackup() {
    SetColor(11); cout << "\n--- Driver Backup Utility ---" << endl; SetColor(7);
    cout << "This will export all 3rd party drivers to C:\\DriverBackup" << endl;
    cout << "Confirm? (y/n): "; char c; cin >> c;
    if (c == 'y') {
        fs::create_directory("C:\\DriverBackup");
        RunLongTask("dism /online /export-driver /destination:C:\\DriverBackup", "Exporting Drivers");
        SetColor(10); cout << ">> Drivers saved to C:\\DriverBackup" << endl;
    }
    Pause();
}

void ModuleNetworkTweaks() {
    SetColor(11); cout << "\n--- Network Optimizer ---" << endl; SetColor(7);
    cout << "1. Set Cloudflare DNS (1.1.1.1) - Faster Speed" << endl;
    cout << "2. Reset TCP/IP Stack (Fix connection issues)" << endl;
    cout << "3. Flush DNS Cache" << endl;
    cout << "0. Cancel" << endl;
    int c; cin >> c;

    if (c == 1) {
        // Hem Wi-Fi hem Ethernet için deneme yapar
        RunLongTask("netsh interface ip set dns \"Wi-Fi\" static 1.1.1.1", "Setting Wi-Fi DNS");
        RunLongTask("netsh interface ip set dns \"Ethernet\" static 1.1.1.1", "Setting Ethernet DNS");
    } else if (c == 2) {
        RunLongTask("netsh int ip reset", "Resetting TCP/IP");
        RunLongTask("netsh winsock reset", "Resetting Winsock");
    } else if (c == 3) {
        system("ipconfig /flushdns");
    }
    if (c != 0) Pause();
}

void ModulePrivacy() {
    SetColor(11); cout << "\n--- Privacy Shield & Telemetry ---" << endl; SetColor(7);
    cout << "1. Disable Windows Telemetry (Registry)" << endl;
    cout << "2. Disable Advertising ID" << endl;
    cout << "0. Cancel" << endl;
    int c; cin >> c;

    if (c == 1) {
        // Registry komutu ile Telemetri kapatma
        string cmd = "reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection\" /v AllowTelemetry /t REG_DWORD /d 0 /f";
        RunLongTask(cmd, "Disabling Telemetry");
    } else if (c == 2) {
        string cmd = "reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AdvertisingInfo\" /v Enabled /t REG_DWORD /d 0 /f";
        RunLongTask(cmd, "Disabling Ad ID");
    }
    if (c != 0) Pause();
}

void ModuleContextMenu() {
    SetColor(11); cout << "\n--- Context Menu Tweaks ---" << endl; SetColor(7);
    cout << "1. Add 'Take Ownership' to Right Click" << endl;
    cout << "0. Cancel" << endl;
    int c; cin >> c;

    if (c == 1) {
        // Take Ownership komutlarını Registry'e ekler
        string cmd1 = "reg add \"HKCR\\*\\shell\\runas\" /ve /t REG_SZ /d \"Take Ownership\" /f";
        string cmd2 = "reg add \"HKCR\\*\\shell\\runas\\command\" /ve /t REG_SZ /d \"cmd.exe /c takeown /f \\\"%1\\\" && icacls \\\"%1\\\" /grant administrators:F\" /f";
        RunLongTask(cmd1 + " && " + cmd2, "Adding Registry Keys");
    }
    if (c != 0) Pause();
}

// =============================================================
//           MENÜLER (STANDARD & PRO)
// =============================================================

void StartStandardMode() {
    int choice;
    do {
        system("cls");
        SetColor(11); cout << "WINTOOL ULTIMATE v6.0 (STANDARD)" << endl; SetColor(7);
        cout << " 1. Schedule Shutdown\n 2. Abort Shutdown\n 3. Check Disk Space\n 4. Clear Clipboard\n 8. SWITCH TO ADMIN MODE\n 0. Back" << endl;
        cout << "\nSelect: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        switch (choice) {
            case 1: { int m; cout << "Minutes: "; cin >> m; system(("shutdown /s /t " + to_string(m * 60)).c_str()); Pause(); break; }
            case 2: system("shutdown /a"); Pause(); break;
            case 3: { // Basit Disk Kontrolü
                ULARGE_INTEGER f, t, tf; GetDiskFreeSpaceExA("C:\\", &f, &t, &tf);
                cout << "Disk C Free: " << (tf.QuadPart / (1024*1024*1024)) << " GB" << endl; Pause(); break; 
            }
            case 4: system("echo off | clip"); cout << "Clipboard cleared."; Pause(); break;
            case 8: RestartAsAdmin(); break;
            case 0: return;
        }
    } while (choice != 0);
}

int main() {
    SetConsoleTitleA("WinTool Ultimate v6.0 - Architect Edition");
    int choice;
    do {
        system("cls");
        SetColor(11);
        cout << R"(
 _  _  _  ___  __  _  _____  ___   ___  __   
( \/ \/ )(  ,)(  )( )(  _  )(  ,) (  ,)(  )  
 \     /  |  \ | \/ |  )(_)(  |  \  |  \ | |_ 
  \/\_/  _)\_)\____/ (_____)(_)\_)(_)\_)(___) v6.0
        )" << endl;
        
        if (IsRunAsAdmin()) { SetColor(10); cout << " [ ADMIN PRIVILEGES ACTIVE ]" << endl; } 
        else { SetColor(12); cout << " [ LIMITED USER MODE ]" << endl; }
        
        SetColor(14); cout << "\n [SYSTEM & MAINTENANCE]" << endl; SetColor(7);
        cout << " 1. System Repair (SFC/DISM with Spinner)" << endl;
        cout << " 2. Modern Deep Clean (std::filesystem)" << endl;
        cout << " 3. Driver Backup (Export Drivers)" << endl;

        SetColor(14); cout << "\n [TWEAKS & PRIVACY]" << endl; SetColor(7);
        cout << " 4. Network Optimizer (DNS/TCP)" << endl;
        cout << " 5. Privacy Shield (Telemetry)" << endl;
        cout << " 6. Context Menu Tweaks" << endl;

        SetColor(11); cout << "\n [GENERAL]" << endl; SetColor(7);
        cout << " 9. Switch User Mode" << endl;
        cout << " 0. Exit" << endl;

        cout << "\nSelect: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        if (choice == 9) { StartStandardMode(); continue; }
        if (choice == 0) break;

        if (!IsRunAsAdmin()) {
            SetColor(12); cout << "\n[!] Requires Admin Rights!" << endl; Pause(); continue;
        }

        switch (choice) {
            case 1: 
                RunLongTask("sfc /scannow", "Scanning System Files"); 
                Pause(); break;
            case 2: ModernDeepClean(); break;
            case 3: ModuleDriverBackup(); break;
            case 4: ModuleNetworkTweaks(); break;
            case 5: ModulePrivacy(); break;
            case 6: ModuleContextMenu(); break;
        }

    } while (choice != 0);

    return 0;
}