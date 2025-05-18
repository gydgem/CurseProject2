
#ifndef NETWORK_APP_WORKSPACE_H
#define NETWORK_APP_WORKSPACE_H

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <string>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <atomic>
#include <boost/range/iterator_range.hpp>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <functional>

namespace fs = boost::filesystem;
namespace bp = boost::process;

class Workspace {
public:
    using ExecuteCallback = std::function<void(const std::string &)>;

    Workspace() = default;

    explicit Workspace(const fs::path &path);

    std::string execute_command(const std::string &command);

    void upload_file(const std::string &filename, const std::string &content);

    std::string download_file(const std::string &filename);

private:
    void validate_path(const fs::path &file_path) const;

    fs::path path_;
    fs::path current_directory_;
};

#endif //NETWORK_APP_WORKSPACE_H