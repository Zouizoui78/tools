#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

namespace tools {

template <typename ValueT, typename ErrorT>
class Result {
public:
    explicit Result(ValueT &&value)
        : _value(std::forward<ValueT>(value)), _is_error(false) {}

    explicit Result(ErrorT error) : _error(error), _is_error(true) {}

    explicit Result(ErrorT error, std::string_view message) _error(error),
        _message(message), _is_error(true) {}

    ValueT &value() const {
        return _value;
    }

    ErrorT error() const {
        return _error;
    }

    std::string_view message() const {
        return _message;
    }

    bool is_error() const {
        return _is_error;
    }

    // Return !is_error().
    explicit operator bool() const {
        return !_is_error;
    }

private:
    union {
        ValueT _value;
        struct {
            ErrorT _error;
            std::string _message;
        };
    };

    bool _is_error;
};

} // namespace tools

#endif