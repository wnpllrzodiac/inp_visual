#pragma once

#include <QObject>
#include <QString>
#include <fmt/format.h>

template <typename T>
concept PointT = requires(T t) {
    t.x();
    t.y();
};

template <typename T>
concept SizeT = requires(T t) {
    t.width();
    t.height();
};

template <typename T>
concept RectT = requires(T t) {
    t.x();
    t.y();
    t.width();
    t.height();
};

template <> struct fmt::formatter<QString> {
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }
    auto format(const QString& value, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", value.toStdString());
    }
};

template <PointT T> struct fmt::formatter<T> {
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }
    auto format(const T& value, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "({}, {})", value.x(), value.y());
    }
};

template <SizeT T> struct fmt::formatter<T> {
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }
    auto format(const T& value, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "({}, {})", value.width(), value.height());
    }
};

template <RectT T> struct fmt::formatter<T> {
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }
    auto format(const T& value, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "({}, {}, {}, {})", value.x(), value.y(), value.width(), value.height());
    }
};
