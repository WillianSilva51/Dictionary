#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <functional>

class TextProcessor
{
private:
    std::ifstream file_stream;

    std::string normalize(const std::string &word) const;

public:
    explicit TextProcessor(const std::string &input_file);

    ~TextProcessor() = default;

    static void toLowerCase(std::string &text);

    void processFile(const std::function<void(const std::string &)> &wordHandler);
};