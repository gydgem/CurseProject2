//
// Created by gydgem on 5/15/2025.
//

#include "RequestHandler.h"
#include <algorithm>

std::string RequestHandler::process(const std::string& request) {
    std::string response = request;
    std::transform(response.begin(), response.end(), response.begin(), ::toupper);
    return "ECHO: " + response;
}
