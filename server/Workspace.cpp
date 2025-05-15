#include <iostream>
#include "Workspace.h"


namespace fs = boost::filesystem;
namespace bp = boost::process;

// Статический счетчик для генерации уникальных ID сессий
static std::atomic<int> session_counter{0};

void Workspace::validate_path(const fs::path &file_path) const {
    // Проверяем что путь находится внутри рабочей директории
    auto relative = fs::relative(file_path, path_);
    if (relative.empty() || *relative.begin() == "..") {
        throw std::runtime_error("Invalid file path");
    }
}

// Workspace.cpp (исправленный метод execute_command)
std::string Workspace::execute_command(const std::string &command) {
    try {
        bp::ipstream stdout_stream;
        bp::ipstream stderr_stream;
        bp::child c(
                "/bin/bash",  // Явно указываем bash
                bp::args({"-c", command}),
                bp::std_out > stdout_stream,
                bp::std_err > stderr_stream,  // Разделяем stdout и stderr
                bp::start_dir(path_.string()),
                bp::std_in.close()  // Закрываем stdin
        );

        // Читаем вывод асинхронно
        std::stringstream output;
        std::string line;

        // Читаем stdout
        while (stdout_stream && std::getline(stdout_stream, line)) {
            output << line << "\n";
        }

        // Читаем stderr
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
}


