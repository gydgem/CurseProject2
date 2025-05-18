#include "ZoneManager.h"
#include <iostream>

ZoneManager::ZoneManager() {
    fs::create_directories(base_path_);
    scan_existing_zones();
}

void ZoneManager::scan_existing_zones() {
    std::lock_guard<std::mutex> lock(mutex_);
    zones_.clear();

    for (const auto &entry: fs::directory_iterator(base_path_)) {
        if (fs::is_directory(entry)) {
            std::string name = entry.path().filename().string();
            zones_[name] = {name, false};
        }
    }
}

bool ZoneManager::create_zone(const std::string &name) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (zones_.count(name)) return false;

    fs::path zone_path = base_path_ / name;
    if (!fs::create_directories(zone_path)) return false;

    zones_[name] = {name, false};
    return true;
}

bool ZoneManager::enter_zone(const std::string &name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = zones_.find(name);
    if (it == zones_.end() || it->second.occupied) return false;

    it->second.occupied = true;
    return true;
}

void ZoneManager::leave_zone(const std::string &name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = zones_.find(name);
    if (it != zones_.end()) {
        it->second.occupied = false;
    }
}

std::vector<std::string> ZoneManager::list_zones() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::string> result;
    for (const auto &[name, info]: zones_) {
        result.push_back(name + " - " + (info.occupied ? "occupied" : "available"));
    }
    return result;
}

fs::path ZoneManager::get_zone_path(const std::string &name) const {
    return base_path_ / name;
}