#include <PIDSource.h>
#include <PIDOutput.h>
#include <atomic>

class PIDInputVariable : public PIDSource {
    std::atomic<double> value;
    double PIDGet() {
        return value;
    }
    PIDInputVariable &operator=(double value) {
        this->value = value;
        return *this;
    } 
    operator double() {
        return value;
    }
};

class PIDOutputVariable : public PIDOutput {
    std::atomic<double> value;
    void PIDWrite(double output) {
        value = output;
    }
    operator double(){
        return value;
    }
};