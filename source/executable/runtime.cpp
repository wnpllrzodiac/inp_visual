#include "runtime.h"
#include "settings.h"

Runtime::Runtime()
{
    settings_ = new Settings;
}

Runtime::~Runtime() noexcept
{
    delete settings_;
    settings_ = nullptr;
}

Runtime& Runtime::instance()
{
    static Runtime ins;
    return ins;
}

bool Runtime::initialize()
{
    instance().settings_->initialize();
    return true;
}

Settings& Runtime::settings()
{
    return *Runtime::instance().settings_;
}