#include <string>

#include "SmartDashboard/SmartDashboard.h"

namespace Lib830 {
namespace Dashboard {

class Widget {
public:
    virtual void sendToDashboard (std::string id) = 0;
};

}}
