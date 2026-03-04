#include "exercise.hpp"

Device::Device(std::string name) : name_(std::move(name)) {}

const std::string& Device::name() const noexcept {
    return name_;
}

std::string Device::read() const {
    return "Device[" + name_ + "]::read";
}

void Device::write(const std::string& data) const {
    std::cout << "Device[" << name_ << "]::write: " << data << "\n";
}

InputDevice::InputDevice(std::string name) : Device(std::move(name)) {}

std::string InputDevice::read() const {
    return "InputDevice[" + name() + "]::read";
}

OutputDevice::OutputDevice(std::string name) : Device(std::move(name)) {}

void OutputDevice::write(const std::string& data) const {
    std::cout << "OutputDevice[" << name() << "]::write: " << data << "\n";
}

IODevice::IODevice(std::string name) : Device(std::move(name)), InputDevice(""), OutputDevice("") {}

std::string IODevice::read() const {
    return "IODevice[" + name() + "]::read";
}

void IODevice::write(const std::string& data) const {
    std::cout << "IODevice[" << name() << "]::write: " << data << "\n";
}

void demo_io_device(IODevice& dev) {
    std::string result = dev.read();
    std::cout << "demo read: " << result << "\n";
    dev.write("hello");
}
