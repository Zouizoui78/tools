#ifndef ISERVICE_HPP
#define ISERVICE_HPP

namespace tools {

enum class ServiceState {
    STOPPED,
    STARTING,
    RUNNING,
    STOPPING,
    FAILED
};

// Interface for an object that can be started and stopped and goes through the
// different states from ServiceState.
// The state sequence is as follows:
// STOPPED (default state) -> start() -> STARTING -> RUNNING -> stop() ->
// STOPPING -> STOPPED The FAILED state represents a state of unrecoverable
// error the service as fallen into.
class IService {
public:
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual ServiceState get_state() const = 0;
    virtual bool is_running_or_starting() const = 0;
    virtual bool is_stopped_or_stopping() const = 0;

    virtual ~IService() = default;
};

} // namespace tools

#endif