#pragma once

class Settings;

class Runtime {
public:
    ~Runtime() noexcept;

    static bool initialize();

    static Settings& settings();

private:
    static Runtime& instance();
    Runtime();

private:
    Settings* settings_ { nullptr };
};