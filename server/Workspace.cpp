#include <iostream>
#include "Workspace.h"


namespace fs = boost::filesystem;
namespace bp = boost::process;

static std::atomic<int> session_counter{0};

void Workspace::validate_path(const fs::path &file_path) const {
    auto relative = fs::relative(file_path, path_);
    if (relative.empty() || *relative.begin() == "..") {
        throw std::runtime_error("Invalid file path");
    }
}

std::string Workspace::execute_command(const std::string &command) {
    if (command.substr(0, 3) == "cd ") {
        std::string dir = command.substr(3);
        fs::path new_dir = (current_directory_ / dir).lexically_normal();

        auto relative = fs::relative(new_dir, path_);
        if (relative.empty() || *relative.begin() == "..") {
            return "Error: Cannot change directory outside workspace.\n";
        }

        if (!fs::exists(new_dir) || !fs::is_directory(new_dir)) {
            return "Error: Directory does not exist.\n";
        }

        current_directory_ = new_dir;
        return "Changed directory to: " + current_directory_.string() + "\n";
    } else {
        try {
            bp::ipstream stdout_stream;
            bp::ipstream stderr_stream;
            bp::child c(
                    "/bin/bash",
                    bp::args({"-c", command}),
                    bp::std_out > stdout_stream,
                    bp::std_err > stderr_stream,
                    bp::start_dir(current_directory_.string()),
                    bp::std_in.close()
            );

            std::stringstream output;
            std::string line;

            while (stdout_stream && std::getline(stdout_stream, line)) {
                output << line << "\n";
            }

            while (stderr_stream && std::getline(stderr_stream, line)) {
                output << "[stderr] " << line << "\n";
            }

            c.wait();
            output << "\nExit code: " << c.exit_code();
            return output.str();
        } catch (const bp::process_error &e) {
            return "Process error: " + std::string(e.what());
        }
    }
}


void Workspace::upload_file(const std::string &filename, const std::string &content) {
    fs::path file_path = path_ / filename;
    validate_path(file_path);

    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing");
    }

    file << content;
}

std::string Workspace::download_file(const std::string &filename) {
    fs::path file_path = path_ / filename;
    validate_path(file_path);

    if (!fs::exists(file_path)) {
        throw std::runtime_error("File not found");
    }

    std::ifstream file(file_path, std::ios::binary);
    return std::string(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
    );
}

Workspace::Workspace() {
    fs::create_directories("workspaces");
    path_ = fs::path("workspaces") / ("session_" + std::to_string(++session_counter));
    fs::create_directories(path_);
    current_directory_ = path_;
}


