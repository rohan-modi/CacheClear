#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <vector>

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

        // Separate files and directories
        std::vector<fs::directory_entry> files;
        std::vector<fs::directory_entry> directories;

        std::vector<std::pair<int, std::string>> filesInfo;
        std::vector<std::pair<int, std::string>> directoriesInfo;

        for (const auto& entry : fs::directory_iterator(current_directory)) {
            if (fs::is_regular_file(entry.status())) {
                files.push_back(entry);
            } else if (fs::is_directory(entry.status())) {
                directories.push_back(entry);
            }
        }

        // Sort files first
        for (const auto& file : files) {
            filesInfo.push_back({fs::file_size(file), file.path().filename().string()});
            // std::cout << file.path().filename().string();
            // std::cout << " (Size: " << fs::file_size(file) << " bytes)";
            // std::cout << std::endl;
        }

        std::sort(filesInfo.begin(), filesInfo.end(),
        [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
            return a.first > b.first;  // Sorting by integer in descending order
        });

        for (int i = 0; i < filesInfo.size(); i++) {
            std::cout << filesInfo[i].second;
            std::cout << " (Size: " << filesInfo[i].first << " bytes)";
            std::cout << std::endl;
        }

        // Output directories
        for (const auto& directory : directories) {
            std::uintmax_t dir_size = calculate_directory_size(directory.path(), max_depth, max_files, max_size, 1, &file_count, &exceeded_limit);
            // std::cout << directory.path().filename().string();
            // std::cout << " (Directory, Size: " << dir_size << " bytes)";
            // std::cout << std::endl;

            directoriesInfo.push_back({dir_size, directory.path().filename().string()});

            if (exceeded_limit) {
                std::cout << "Execution stopped due to one of the limits being reached." << std::endl;
                break;
            }
        }

        std::sort(directoriesInfo.begin(), directoriesInfo.end(),
        [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
            return a.first > b.first;  // Sorting by integer in descending order
        });

        for (int i = 0; i < directoriesInfo.size(); i++) {
            std::cout << directoriesInfo[i].second;
            std::cout << " Directory (Size: " << directoriesInfo[i].first << " bytes)";
            std::cout << std::endl;
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
