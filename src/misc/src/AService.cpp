#include "tools/misc/AService.hpp"

namespace tools {

ServiceState AService::get_state() const {
    return _state;
}

bool AService::is_stopped() const {
    return _state == ServiceState::STOPPED;
}

bool AService::is_starting() const {
    return _state == ServiceState::STARTING;
}

bool AService::is_running() const {
    return _state == ServiceState::RUNNING;
}

bool AService::is_stopping() const {
    return _state == ServiceState::STOPPING;
}

bool AService::is_failed() const {
    return _state == ServiceState::FAILED;
}

bool AService::is_running_or_starting() const {
    return _state == ServiceState::RUNNING || _state == ServiceState::STARTING;
}

bool AService::is_stopped_or_stopping() const {
    return _state == ServiceState::STOPPED || _state == ServiceState::STOPPING;
}

} // namespace tools
