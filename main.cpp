#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <limits>
#include <shellapi.h> // ShellExecute için gerekli

using namespace std;

// =============================================================
//               YARDIMCI FONKSIYONLAR
// =============================================================

void SetColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void Pause() {
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Admin Kontrolü (G++ Uyumlu)
bool IsRunAsAdmin() {
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdminSID = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSID)) {
        if (!CheckTokenMembership(NULL, pAdminSID, &fIsRunAsAdmin)) {
            fIsRunAsAdmin = FALSE;
        }
        FreeSid(pAdminSID);
    }
    return fIsRunAsAdmin;
}

// *** YENİ: YÖNETİCİ OLARAK YENİDEN BAŞLATMA ***
void RestartAsAdmin() {
    char szPath[MAX_PATH];
    
    // 1. Mevcut .exe dosyasının yolunu bul
    if (GetModuleFileNameA(NULL, szPath, ARRAYSIZE(szPath))) {
        
        // 2. Aynı dosyayı "runas" (Yönetici) parametresiye çalıştır
        SetColor(14);
        cout << "\n[!] Requesting Administrator Privileges..." << endl;
        cout << "[!] Please click 'Yes' on the UAC popup." << endl;
        
        HINSTANCE result = ShellExecuteA(NULL, "runas", szPath, NULL, NULL, SW_SHOW);

        // 3. Eğer işlem başarılıysa (Değer > 32 ise) bu pencreyi kapat
        if ((intptr_t)result > 32) {
            exit(0); // Eski kısıtlı uygulamayı kapat
        } else {
            SetColor(12);
            cout << "\n[ERROR] Admin request cancelled by user." << endl;
            SetColor(7);
            Pause();
        }
    }
}

// =============================================================
//           BOLUM 1: STANDART MOD (Kısıtlı Kullanıcı)
// =============================================================

void HeaderStandard() {
    system("cls");
    SetColor(11); 
    cout << "================================================" << endl;
    cout << "       WINDOWS UTILITY TOOLKIT (v5.0 STD)       " << endl;
    cout << "      Standard Mode (No Admin Required)         " << endl;
    cout << "================================================" << endl;
    SetColor(7);
}

void CheckDiskSpace() {
    ULARGE_INTEGER freeBytes, totalBytes, totalFree;
    if (GetDiskFreeSpaceExA("C:\\", &freeBytes, &totalBytes, &totalFree)) {
        double totalGB = (double)totalBytes.QuadPart / (1024 * 1024 * 1024);
        double freeGB = (double)totalFree.QuadPart / (1024 * 1024 * 1024);
        cout << "\nDisk C: Total: " << totalGB << "GB | Free: " << freeGB << "GB" << endl;
    }
    Pause();
}

void StartStandardMode() {
    int choice;
    do {
        HeaderStandard();
        SetColor(15); cout << " [STANDARD MENU]" << endl; SetColor(7);
        cout << " 1. Schedule Shutdown" << endl;
        cout << " 2. Abort Shutdown" << endl;
        cout << " 3. Check Disk Space" << endl;
        cout << " 4. Clear Clipboard" << endl;
        
        SetColor(14); // Sarı Dikkat çekici renk
        cout << "\n 8. SWITCH TO ADMINISTRATOR MODE (Re-Launch)" << endl;
        
        SetColor(8); // Gri
        cout << " 0. Back to Main Menu" << endl;
        
        cout << "\nSelect: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        switch (choice) {
            case 1: { int m; cout << "Minutes: "; cin >> m; system(("shutdown /s /t " + to_string(m * 60)).c_str()); Pause(); break; }
            case 2: system("shutdown /a"); Pause(); break;
            case 3: CheckDiskSpace(); break;
            case 4: system("echo off | clip"); cout << "Clipboard cleared."; Pause(); break;
            
            // BURASI YENİ ÖZELLİK
            case 8: RestartAsAdmin(); break;
            
            case 0: return;
        }
    } while (choice != 0);
}

// =============================================================
//           BOLUM 2: PRO MOD (Yönetici)
// =============================================================

void HeaderPro() {
    system("cls");
    SetColor(11);
    cout << "================================================" << endl;
    cout << "       WINTOOL ULTIMATE (ADMIN EDITION)         " << endl;
    cout << "================================================" << endl;
    
    if (IsRunAsAdmin()) {
        SetColor(10); cout << " [STATUS: ADMINISTRATOR PRIVILEGES ACTIVE]" << endl;
    } else {
        SetColor(12); cout << " [STATUS: LIMITED USER]" << endl;
    }
    SetColor(7);
}

int main() {
    SetConsoleTitleA("WinTool Ultimate v5.0");
    
    int choice;
    do {
        HeaderPro();
        
        if (IsRunAsAdmin()) {
            SetColor(14); cout << " [ADMIN TOOLS]" << endl; SetColor(7);
            cout << " 1. System Repair (SFC/DISM)" << endl;
            cout << " 2. Software Installer (Winget)" << endl;
            cout << " 3. Performance Tweaks (Power)" << endl;
        } else {
            SetColor(8); // Gri (Devre dışı gibi görünsün)
            cout << " [ADMIN TOOLS - LOCKED]" << endl;
            cout << " 1. System Repair (Requires Admin)" << endl;
            cout << " 2. Software Installer (Requires Admin)" << endl;
            cout << " 3. Performance Tweaks (Requires Admin)" << endl;
        }

        SetColor(11); cout << "\n [GENERAL]" << endl; SetColor(7);
        cout << " 9. Standard Mode / Switch User" << endl;
        cout << " 0. Exit" << endl;
        
        cout << "\nSelect: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        // Eğer kullanıcı Admin değilse ve Admin özellikleri seçerse uyar
        if (!IsRunAsAdmin() && (choice >= 1 && choice <= 3)) {
            SetColor(12);
            cout << "\n[!] Access Denied. You are not Admin." << endl;
            cout << "    Go to Option 9 -> Select 'Switch to Administrator Mode'." << endl;
            Pause();
            continue;
        }

        if (choice == 9) { StartStandardMode(); continue; }
        if (choice == 0) break;

        // Sadece Admin ise çalışacak kodlar
        switch (choice) {
            case 1: system("sfc /scannow"); Pause(); break;
            case 2: system("winget install -e --id Google.Chrome"); Pause(); break;
            case 3: system("powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61"); Pause(); break;
        }

    } while (choice != 0);

    return 0;
}