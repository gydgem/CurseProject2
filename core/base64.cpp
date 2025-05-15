//
// Created by gydgem on 5/15/2025.
//

#include "base64.h"

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

std::string base64_encode(const std::string &in, bool with_new_lines) {
    std::string out;
    int val = 0, valb = -6;
    size_t line_len = 0;

    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
            line_len++;

            // Вставляем перенос каждые 76 символов
            if (with_new_lines && line_len >= 76) {
                out.push_back('\n');
                line_len = 0;
            }
        }
    }

    // Добиваем оставшиеся биты
    if (valb > -6) {
        out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        line_len++;
    }

    // Добавляем padding
    while (out.size() % 4 != 0) {
        out.push_back('=');
        line_len++;
    }

    return out;
}

std::string base64_decode(const std::string &in, bool with_new_lines) {
    std::string out;
    std::vector<int> T(256, -1);

    // Заполняем таблицу преобразования
    for (int i = 0; i < 64; i++) {
        T[base64_chars[i]] = i;
    }
    T['='] = 0;

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        // Пропускаем переносы строк при необходимости
        if (c == '\n' && with_new_lines) continue;

        if (T[c] == -1) {
            // Недопустимый символ
            return "";
        }

        val = (val << 6) + T[c];
        valb += 6;

        if (valb >= 0) {
            out.push_back(static_cast<char>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}
