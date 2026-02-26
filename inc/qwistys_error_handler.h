#ifndef QWISTYS_ERROR_HANDLER_H
#define QWISTYS_ERROR_HANDLER_H

/**
 * Shared error-handling machinery for all qwistys projects.
 *
 * Usage:
 *   1. Define your project-specific ErrorCode enum.
 *   2. Optionally define QWISTYS_ERROR_MSG(msg, ...) before this include
 *      (e.g. to route through a project logger). A fprintf fallback is
 *      provided if the macro is not already defined.
 *   3. #include "qwistys_error_handler.h"
 *
 * The header intentionally does NOT define ErrorCode — each project owns
 * its own error codes.
 */

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

// Logging fallback — override before including this header if desired.
#ifndef QWISTYS_ERROR_MSG
#    include <stdio.h>
#    define QWISTYS_ERROR_MSG(msg, ...) fprintf(stderr, "[ERROR] " msg "\n", ##__VA_ARGS__)
#endif

/**
 * @brief Error severity for disaster recovery classification.
 */
enum class Severity { LOW, MEDIUM, HIGH, CRITICAL };

/**
 * @brief Error object: holds code, human-readable message, and severity.
 *
 * ErrorCode must be defined by the including project before this header.
 */
class Error {
public:
    Error(ErrorCode code, const std::string& message, Severity severity)
        : code_(code), message_(message), severity_(severity) {}

    ErrorCode code() const { return code_; }
    const std::string& message() const { return message_; }
    Severity severity() const { return severity_; }

private:
    ErrorCode code_;
    std::string message_;
    Severity severity_;
};

/**
 * @brief Result<T> — typed return value or error.
 */
template <typename T>
class Result {
public:
    explicit Result(T value) : value_(std::move(value)) {}
    explicit Result(Error error) : _error(std::move(error)) {}

    bool is_ok() const { return !_error.has_value(); }
    bool is_err() const { return _error.has_value(); }

    const T& value() const {
        if (_error)
            throw std::runtime_error("Result contains an error");
        return *value_;
    }
    const Error& error() const {
        if (!_error)
            throw std::runtime_error("Result does not contain an error");
        return *_error;
    }

private:
    std::optional<T> value_;
    std::optional<Error> _error;
};

template <typename T>
inline Result<T> Ok(T value) {
    return Result<T>(std::move(value));
}

template <typename T>
inline Result<T> Err(ErrorCode code, const std::string& message,
                     Severity severity = Severity::MEDIUM) {
    return Result<T>(Error(code, message, severity));
}

/**
 * @brief VoidResult — success/error for void-returning functions.
 */
class VoidResult {
public:
    explicit VoidResult() = default;
    explicit VoidResult(Error error) : _error(std::move(error)) {}

    bool is_ok() const { return !_error.has_value(); }
    bool is_err() const { return _error.has_value(); }

    const Error& error() const {
        if (!_error)
            throw std::runtime_error("VoidResult does not contain an error");
        return *_error;
    }

private:
    std::optional<Error> _error;
};

inline VoidResult Ok() {
    return VoidResult();
}

inline VoidResult Err(ErrorCode code, const std::string& message,
                      Severity severity = Severity::MEDIUM) {
    return VoidResult(Error(code, message, severity));
}

/**
 * @brief DisasterRecoveryPlan — register recovery callbacks per error code.
 */
class DisasterRecoveryPlan {
public:
    using RecoveryAction = std::function<bool()>;

    void register_recovery_action(ErrorCode code, RecoveryAction action) {
        _recovery_actions[code] = std::move(action);
    }

    bool execute_recovery(const Error& error) {
        if (auto it = _recovery_actions.find(error.code()); it != _recovery_actions.end()) {
            return it->second();
        }
        return false;
    }

private:
    std::unordered_map<ErrorCode, RecoveryAction> _recovery_actions;
};

/**
 * @brief ErrorHandler — dispatch callbacks and drive DRP on errors.
 *
 * CRITICAL errors that cannot be recovered abort the process.
 */
class ErrorHandler {
public:
    using ErrorCallback = std::function<void(const Error&)>;

    explicit ErrorHandler(DisasterRecoveryPlan& drp) : drp_(drp) {}

    void register_callback(ErrorCode code, ErrorCallback callback) {
        callbacks_[code] = std::move(callback);
    }

    bool handle_error(const Error& error) const {
        if (auto it = callbacks_.find(error.code()); it != callbacks_.end()) {
            it->second(error);
        }

        bool recovered = false;
        if (error.severity() >= Severity::HIGH) {
            recovered = drp_.execute_recovery(error);
            if (!recovered && error.severity() == Severity::CRITICAL) {
                QWISTYS_ERROR_MSG("BAD ERROR: %s", error.message().c_str());
                std::abort();
            }
        }

        log_error(error);
        return recovered;
    }

private:
    std::unordered_map<ErrorCode, ErrorCallback> callbacks_;
    DisasterRecoveryPlan& drp_;

    void log_error(const Error& error) const {
        QWISTYS_ERROR_MSG("[ Severity: %d Error code: %d Error message: %s ]",
                          static_cast<int>(error.severity()),
                          static_cast<int>(error.code()),
                          error.message().c_str());
    }
};

#endif  // QWISTYS_ERROR_HANDLER_H
