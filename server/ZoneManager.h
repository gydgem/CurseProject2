#ifndef NETWORK_APP_ZONEMANAGER_H
#define NETWORK_APP_ZONEMANAGER_H

#include <boost/filesystem.hpp>
#include <map>
#include <mutex>
#include <vector>

namespace fs = boost::filesystem;

struct ZoneInfo {
    std::string name;
    bool occupied;
};

class ZoneManager {
public:
    ZoneManager();

    bool create_zone(const std::string &name);

    bool enter_zone(const std::string &name);

    void leave_zone(const std::string &name);

    std::vector<std::string> list_zones() const;

    fs::path get_zone_path(const std::string &name) const;

private:
    void scan_existing_zones();

    mutable std::mutex mutex_;
    std::map<std::string, ZoneInfo> zones_;
    fs::path base_path_ = "workspaces";
};

#endif //NETWORK_APP_ZONEMANAGER_H