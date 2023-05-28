#include <iostream>
#include <vector>
using namespace std;

// Клас пристрою
class Device {
public:
    Device(string name) : m_name(name), m_state(false) {}

    string getName() const {
        return m_name;
    }

    bool getState() const {
        return m_state;
    }

    void setState(bool state) {
        m_state = state;
    }

private:
    string m_name;
    bool m_state;
};

// Інтерфейс для спостерігачів
class DeviceObserver {
public:
    void update(string deviceName, bool state) {
        cout << "Device " << deviceName << " changed state to " << state << endl;
    }
    void update(string message) {
        cout << message << endl;
    }
};

// Клас менеджера пристроїв
class DeviceManager {
public:
    static DeviceManager* getInstance() {
        static DeviceManager instance;
        return &instance;
    }

    void addDevice(Device* device) {
        m_devices.push_back(device);
        notifyObservers("New device " + device->getName() + " added");
    }

    void removeDevice(string deviceName) {
        notifyObservers("Device " + deviceName + " removed");
        for (auto i = m_devices.begin(); i != m_devices.end(); i++) {
            if ((*i)->getName() == deviceName) {
                m_devices.erase(i);
                return;
            }
        }
    }

    void registerObserver(DeviceObserver* observer) {
        m_observers.push_back(observer);
    }

    void notifyObservers(const string& message) {
        for (auto observer : m_observers) {
            observer->update(message);
        }
    }

    void notifyObservers(string deviceName, bool state) {
        for (auto observer : m_observers) {
            observer->update(deviceName, state);
        }
    }

    void updateDeviceState(string deviceName, bool state) {
        for (auto device : m_devices) {
            if (device->getName() == deviceName) {
                device->setState(state);
                notifyObservers(deviceName, state);
                return;
            }
        }
    }

    enum OperationType { START, STOP };

    void OperationOnAllDevices(OperationType operation) {
        for (auto device : m_devices) {
            switch (operation) {
            case OperationType::START:
                device->setState(true);
                notifyObservers(device->getName(), true);
                break;
            case OperationType::STOP:
                device->setState(false);
                notifyObservers(device->getName(), false);
                break;
            }
        }
    }

private:
    DeviceManager() {}
    vector<Device*> m_devices;
    vector<DeviceObserver*> m_observers;
};

int main() {

    // Реєстрація спостерігача в DeviceManager
    DeviceManager::getInstance()->registerObserver(new DeviceObserver());

    // Додавання нових пристроїв
    DeviceManager::getInstance()->addDevice(new Device("Device1"));
    DeviceManager::getInstance()->addDevice(new Device("Device2"));
    DeviceManager::getInstance()->addDevice(new Device("Device3"));

    // Оновлення стану пристрою
    DeviceManager::getInstance()->updateDeviceState("Device1", true);

    // Операція оновлення стану на всіх пристроях
    DeviceManager::getInstance()->OperationOnAllDevices(DeviceManager::OperationType::STOP);

    // Видалення пристроїв
    DeviceManager::getInstance()->removeDevice("Device2");
    DeviceManager::getInstance()->removeDevice("Device1");

    DeviceManager::getInstance()->OperationOnAllDevices(DeviceManager::OperationType::START);

    return 0;
}
