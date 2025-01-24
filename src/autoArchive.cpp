#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

// Check if a file is older than a specified duration
bool isOlderThan(const fs::path& filePath, int days) {
    auto lastWriteTime = fs::last_write_time(filePath);
    auto now = fs::file_time_type::clock::now();
    auto age = std::chrono::duration_cast<std::chrono::hours>(now - lastWriteTime).count();
    return age > (days * 24); // Convert days to hours
}

// Compress files in the given directory while maintaining directory structure
void archiveDirectory(const fs::path& directory, int days) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            // Recurse into subdirectory
            archiveDirectory(entry.path(), days);
        } else if (entry.is_regular_file()) {
            auto filePath = entry.path();
            if (isOlderThan(filePath, days)) {
                // Create a zip archive for this directory (if not already created)
                std::string zipName = (directory / "archive.zip").string();
                std::string command = "zip -j \"" + zipName + "\" \"" + filePath.string() + "\"";

                std::cout << "Archiving: " << filePath << "\n";
                int result = std::system(command.c_str());
                if (result == 0) {
                    fs::remove(filePath); // Delete the file after successful compression
                    std::cout << "Deleted: " << filePath << "\n";
                } else {
                    std::cerr << "Failed to archive: " << filePath << "\n";
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    fs::path directory = fs::current_path(); // Use the current directory
    int days = 30; // Archive files older than 30 days by default

    // Check if an argument is provided for days
    if (argc > 1) {
        try {
            days = std::stoi(argv[1]); // Convert argv[1] to an integer
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument for days: " << argv[1] << ". Using default value of 30 days.\n";
        } catch (const std::out_of_range& e) {
            std::cerr << "Argument for days out of range: " << argv[1] << ". Using default value of 30 days.\n";
        }
    }

    std::cout << "Auto-archiving files in: " << directory << "\n";
    archiveDirectory(directory, days);

    return 0;
}
