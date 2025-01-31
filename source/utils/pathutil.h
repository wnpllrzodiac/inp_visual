#pragma once

#include <string>
#include <string_view>

class PathUtil {
public:
    static std::string getInstallRoot();
    static std::string getInstallFilePath(std::string_view relative_path);

    static std::string getUserRoot(bool ensure_exists = true);
    static std::string getUserFilePath(std::string_view relative_path, bool ensure_parent_exists = true);

    static bool exists(std::string_view path);
    static bool create(std::string_view path, bool is_directory = true);
    static bool remove(std::string_view path);
    static bool createParent(std::string_view path);

    static bool saveToFile(std::string_view path, std::string_view content);
};