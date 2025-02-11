#ifndef ASERVICE_HPP
#define ASERVICE_HPP

#include <atomic>

#include "IService.hpp"

namespace tools {

class AService : public IService {
protected:
    std::atomic<ServiceState> _state = ServiceState::STOPPED;

public:
    ServiceState get_state() const override;
    bool is_running_or_starting() const override;
    bool is_stopped_or_stopping() const override;
};

} // namespace tools

#endif