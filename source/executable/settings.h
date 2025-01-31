#pragma once

#include <QVariantMap>
#include <optional>

#include <QSettings>

class Settings {
public:
    struct StandardConfig {
        // 电机温度刷新间隔（int, 单位：ms）
        static constexpr std::string_view kStatusTempRefreshInterval = "StatusView/TempRefreshInterval";
        // 温度警戒值（float）
        static constexpr std::string_view kStatusTempAlarmValue = "StatusView/TempAlarmValue";
        // 电机温度最小值（float）
        static constexpr std::string_view kStatusTempMinimumValue = "StatusView/TempMinimumValue";
        // 电机温度最大值（float）
        static constexpr std::string_view kStatusTempMaximumValue = "StatusView/TempMaximumValue";
        // 姿态数据刷新间隔（int, 单位：ms）
        static constexpr std::string_view kStatusMotionRefreshInterval = "StatusView/MotionRefreshInterval";

        // 机械臂IP地址
        static constexpr std::string_view kConnectionRoboticArmAddress = "Connection/RoboticArmIP";
        // 控制终端端口号
        static constexpr std::string_view kConnectionTerminalPort = "Connection/TerminalPort";
        // 跟焦器IP地址
        static constexpr std::string_view kConnectionFocuserAddress = "Connection/FocuserIP";

        // 机械臂后端日志路径
        static constexpr std::string_view kPathRoboticArmBackendLogPath = "Path/RoboticArmBackendLogPath";

        static constexpr std::string_view kBackendRobotMoveForwardDelta = "Backend/RobotMoveForwardDelta";
        static constexpr std::string_view kBackendRobotMoveBackwardDelta = "Backend/RobotMoveBackwardDelta";
        static constexpr std::string_view kBackendRobotMoveLeftwardDelta = "Backend/RobotMoveLeftwardDelta";
        static constexpr std::string_view kBackendRobotMoveRightwardDelta = "Backend/RobotMoveRightwardDelta";
        static constexpr std::string_view kBackendRobotMoveUpwardDelta = "Backend/RobotMoveUpwardDelta";
        static constexpr std::string_view kBackendRobotMoveDownwardDelta = "Backend/RobotMoveDownwardDelta";

        static constexpr std::string_view kBackendRobotTiltUpDelta = "Backend/RobotTiltUpDelta";
        static constexpr std::string_view kBackendRobotTiltDownDelta = "Backend/RobotTiltDownDelta";
        static constexpr std::string_view kBackendRobotPanLeftDelta = "Backend/RobotPanLeftDelta";
        static constexpr std::string_view kBackendRobotPanRightDelta = "Backend/RobotPanRightDelta";
        static constexpr std::string_view kBackendRobotRollClockwiseDelta = "Backend/RobotRollClockwiseDelta";
        static constexpr std::string_view kBackendRobotRollCounterClockwiseDelta = "Backend/RobotRollCounterClockwiseDelta";

        static constexpr std::string_view kBackendMoveZoomDelta = "Backend/MoveZoomDelta";

        // 十字键按下之后等待多长时间触发重复点击
        static constexpr std::string_view kInteractiveCrossButtonRepeatDelay = "Interactive/CrossButtonAutoRepeatDelay";
        // 十字键重复点击间隔
        static constexpr std::string_view kInteractiveCrossButtonRepeatInterval = "Interactive/CrossButtonAutoRepeatInterval";
    };

    Settings();

    void initialize();

    bool contains(const QString& key) const;

    template <typename T> std::optional<T> get(const QString& key) const
    {
        if (!settings_.contains(key)) {
            return std::nullopt;
        }
        return settings_.value(key).value<T>();
    }

    template <typename T> void put(const QString& key, const T& value)
    {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, const char*>) {
            settings_.setValue(key, QString(value));
        } else {
            settings_.setValue(key, QVariant::fromValue<T>(value));
        }
    }

    template <typename T> std::optional<T> get(std::string_view key) const
    {
        return get<T>(QString(key.data()));
    }

    template <typename T> std::optional<T> get(const char* key) const
    {
        return get<T>(QString(key));
    }

    template <typename T> void put(std::string_view key, const T& value)
    {
        return put(QString(key.data()), value);
    }

    template <typename T> void put(const char* key, const T& value)
    {
        return put(QString(key), value);
    }

    QStringList allKeys() const
    {
        return settings_.allKeys();
    }

private:
    QSettings settings_;
};