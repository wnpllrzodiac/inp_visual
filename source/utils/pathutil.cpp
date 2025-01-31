#include "pathutil.h"
#include <filesystem>
#include <fstream>

#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#endif

namespace {
#if defined(WIN32) || defined(WIN64)
constexpr std::string_view kEnvUserHome = "USERPROFILE";
#else
constexpr std::string_view kEnvUserHome = "HOME";
#endif

namespace fs = std::filesystem;
} // namespace

std::string PathUtil::getUserRoot(bool ensure_exists)
{
    // #ifdef DEBUG
    return getInstallRoot();
    // #else
    //     const char* user_home = std::getenv(kEnvUserHome.data());
    //     if (!user_home) [[unlikely]] {
    //         return "";
    //     }
    //     auto app_home = fs::canonical(user_home) / ".data_collector";

    //     if (ensure_exists && !create(app_home.generic_string(), true)) {
    //         return "";
    //     }
    //     return app_home.generic_string();
    // #endif
}

std::string PathUtil::getUserFilePath(std::string_view relative_path, bool ensure_parent_exists)
{
    fs::path app_home = getUserRoot(ensure_parent_exists);
    if (app_home.empty()) {
        return relative_path.data();
    }
    auto absolute_path = app_home / relative_path;
    if (ensure_parent_exists) {
        if (!create(absolute_path.parent_path().generic_string(), true)) {
            return relative_path.data();
        }
    }
    return absolute_path.generic_string();
}

bool PathUtil::exists(std::string_view path)
{
    if (path.empty()) {
        return false;
    }
    return fs::exists(path);
}

bool PathUtil::create(std::string_view path, bool is_directory)
{
    try {
        if (fs::exists(path)) {
            return true;
        }
        if (is_directory) {
            return fs::create_directories(path);
        }

        auto parent_path = fs::path(path).parent_path();
        if (!fs::exists(parent_path) && !fs::create_directories(parent_path)) {
            return false;
        }
        std::ofstream ofs(path.data(), std::ios::out);
        ofs.close();
        return fs::exists(path);

    } catch (...) {
        return false;
    }
}

bool PathUtil::remove(std::string_view path)
{
    try {
        if (!exists(path)) {
            return true;
        }
        return fs::remove_all(path) > 0;
    } catch (...) {
        return false;
    }
}

bool PathUtil::createParent(std::string_view path)
{
    try {
        fs::path parent_path = fs::path(path).parent_path();
        if (fs::exists(parent_path) && fs::is_directory(parent_path)) {
            return true;
        }
        if (!fs::is_directory(parent_path) && !remove(parent_path.generic_string())) {
            return false;
        }
        return create(parent_path.generic_string(), true);
    } catch (...) {
        return false;
    }
}

bool PathUtil::saveToFile(std::string_view path, std::string_view content)
{
    try {
        std::ofstream ofs(path.data(), std::ios::out);
        if (!ofs.is_open()) {
            return false;
        }
        ofs << content.data();
        ofs.close();
        return true;
    } catch (...) {
        return false;
    }
}

std::string PathUtil::getInstallRoot()
{
    try {
        fs::path exe_path;
#if defined(WIN32) || defined(WIN64)
        wchar_t path_buffer[MAX_PATH] = { 0 };
        GetModuleFileNameW(nullptr, path_buffer, sizeof(path_buffer) / sizeof(wchar_t));
        exe_path = fs::canonical(path_buffer);
#else
        char path_buffer[PATH_MAX] = { 0 };
        ssize_t len = readlink("proc/self/exe", path_buffer, sizeof(path_buffer) - 1);
        if (len != -1) {
            exe_path = fs::canonical(path_buffer);
        }
#endif
        if (exe_path.empty()) {
            return "";
        }
        return exe_path.parent_path().generic_string();
    } catch (...) {
        return "";
    }
}

std::string PathUtil::getInstallFilePath(std::string_view relative_path)
{
    fs::path install_root = getInstallRoot();
    if (install_root.empty()) {
        return relative_path.data();
    }
    return (install_root / relative_path).generic_string();
}
