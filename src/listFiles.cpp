#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

// Function to calculate the total size of a directory recursively with limits
std::uintmax_t calculate_directory_size(const fs::path& directory, int max_depth, int max_files, std::uintmax_t max_size, int current_depth = 0, int* file_count = nullptr, bool* exceeded_limit = nullptr) {
    std::uintmax_t total_size = 0;
    if (current_depth > max_depth && max_depth >= 0) {
        if (exceeded_limit) *exceeded_limit = true;
        return total_size;
    }

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_regular_file(entry.status())) {
                total_size += fs::file_size(entry);
                if (file_count) (*file_count)++;

                if (max_files >= 0 && file_count && *file_count > max_files) {
                    if (exceeded_limit) *exceeded_limit = true;
                    return total_size;
                }

                if (max_size >= 0 && total_size > max_size) {
                    if (exceeded_limit) *exceeded_limit = true;
                    return total_size;
                }
            } else if (fs::is_directory(entry.status())) {
                total_size += calculate_directory_size(entry.path(), max_depth, max_files, max_size, current_depth + 1, file_count, exceeded_limit);
                if (exceeded_limit && *exceeded_limit) {
                    return total_size;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return total_size;
}

int main(int argc, char* argv[]) {
    int max_depth = -1;
    int max_files = -1;
    std::uintmax_t max_size = -1;

    if (argc > 1) max_depth = std::stoi(argv[1]);
    if (argc > 2) max_files = std::stoi(argv[2]);
    if (argc > 3) max_size = std::stoull(argv[3]);

    fs::path current_directory = fs::current_path();

    try {
        int file_count = 0;
        bool exceeded_limit = false;

        // Iterate through the directory and print file/folder names with sizes
        for (const auto& entry : fs::directory_iterator(current_directory)) {
            std::cout << entry.path().filename().string();

            if (fs::is_regular_file(entry.status())) {
                std::cout << " (Size: " << fs::file_size(entry) << " bytes)";
            } else if (fs::is_directory(entry.status())) {
                std::uintmax_t dir_size = calculate_directory_size(entry.path(), max_depth, max_files, max_size, 1, &file_count, &exceeded_limit);
                std::cout << " (Directory, Size: " << dir_size << " bytes)";
            } else {
                std::cout << " (Other)";
            }

            std::cout << std::endl;

            if (exceeded_limit) {
                std::cout << "Execution stopped due to one of the limits being reached." << std::endl;
                break;
            }
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
