#ifndef ISERVICE_HPP
#define ISERVICE_HPP

namespace tools::utils {

enum class ServiceState {
    STOPPED,
    RUNNING,
    FAILED
};

// Interface for an object that can be started and stopped and goes through the
// different states from ServiceState.
// The state sequence is as follows:
// STOPPED -> start() -> RUNNING -> stop() -> STOPPED
// The FAILED state represents a state of unrecoverable error the service as
// fallen into.
class IService {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ServiceState get_state() const = 0;

    virtual ~IService() = default;
};

} // namespace tools::utils

#endif