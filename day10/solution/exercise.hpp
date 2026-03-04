#pragma once

#include <iostream>
#include <string>

class Device {
  protected:
    std::string name_;

  public:
    explicit Device(std::string name);
    virtual ~Device() = default;

    const std::string& name() const noexcept;
    virtual std::string read() const;
    virtual void write(const std::string& data) const;
};

class InputDevice : public virtual Device {
  public:
    explicit InputDevice(std::string name);
    std::string read() const override;
};

class OutputDevice : public virtual Device {
  public:
    explicit OutputDevice(std::string name);
    void write(const std::string& data) const override;
};

class IODevice : public InputDevice, public OutputDevice {
  public:
    explicit IODevice(std::string name);
    std::string read() const override;
    void write(const std::string& data) const override;
};

void demo_io_device(IODevice& dev);
