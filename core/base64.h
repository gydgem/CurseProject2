//
// Created by gydgem on 5/15/2025.
//

#ifndef NETWORK_APP_BASE64_H
#define NETWORK_APP_BASE64_H

#include <string>
#include <vector>

std::string base64_encode(const std::string &in, bool with_new_lines =false);
std::string base64_decode(const std::string &in,bool with_new_lines = false);

#endif //NETWORK_APP_BASE64_H

