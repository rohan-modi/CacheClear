#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <string>
#include <set>

namespace fs = std::filesystem;

// Function to check if a file is older than 24 hours
bool isOlderThan24Hours(const fs::path& filePath) {
    auto lastWriteTime = fs::last_write_time(filePath);
    auto now = fs::file_time_type::clock::now();
    auto age = std::chrono::duration_cast<std::chrono::hours>(now - lastWriteTime).count();
    return age > 24;
}

// Function to check if a path is in an excluded directory
bool isInExcludedDirectory(const fs::path& filePath, const std::set<std::string>& excludedDirs) {
    for (const auto& dir : excludedDirs) {
        if (filePath.string().find(dir) == 0) {
            return true;
        }
    }
    return false;
}

// Function to confirm deletion
bool confirmDeletion(const fs::path& file) {
    char response;
    std::cout << "Delete file: " << file << "? (y/n): ";
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}

// Main file cleanup function
void cleanFiles(const fs::path& directory, bool force) {
    std::set<std::string> excludedDirs = {"/etc", "/bin", "/usr", "/lib", "/opt", "/sbin"};
    std::vector<fs::path> filesToDelete;

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            auto filePath = entry.path();
            auto extension = filePath.extension().string();

            // Skip sensitive directories and files
            if (isInExcludedDirectory(filePath, excludedDirs)) {
                continue;
            }

            // Target only specific file types
            if (extension == ".tmp" || extension == ".log" || extension == ".cache") {
                // Skip `.tmp` files modified within the last 24 hours
                if (extension == ".tmp" && !isOlderThan24Hours(filePath)) {
                    continue;
                }
                filesToDelete.push_back(filePath);
            }
        }
    }

    // Process deletions
    for (const auto& file : filesToDelete) {
        if (force || confirmDeletion(file)) {
            try {
                fs::remove(file);
                std::cout << "Deleted: " << file << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to delete " << file << ": " << e.what() << "\n";
            }
        } else {
            std::cout << "Skipped: " << file << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    fs::path directory = fs::current_path(); // Always use the current directory
    bool force = (argc > 1 && std::string(argv[1]) == "--force");

    std::cout << "Cleaning files in directory: " << directory << "\n";

    cleanFiles(directory, force);
    return 0;
}
