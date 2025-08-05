#include "text_processor/TextProcessor.hpp"

TextProcessor::TextProcessor(const std::string &input_file) : file_stream(input_file)
{
    if (!file_stream or !file_stream.is_open())
        throw std::runtime_error("CRITICAL ERROR: Could not open file: " + input_file + ".");
}

void TextProcessor::toLowerCase(std::string &text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
                   { return std::tolower(c, std::locale("pt_BR.UTF-8")); });
}

void TextProcessor::processFile(const std::function<void(const std::string &)> &wordHandler)
{
    std::string word{};

    while (file_stream >> word)
    {
        std::string cleanedWord{normalize(word)};

        if (!cleanedWord.empty())
            wordHandler(cleanedWord);
    }
}

std::string TextProcessor::normalize(const std::string &word) const
{
    std::string lowerWord{word};

    toLowerCase(lowerWord);

    static const std::regex word_regex("[a-zà-ÿ]+(?:[-'][a-zà-ÿ]+)*"); // Regex para palavras válidas, incluindo acentos e hífens.

    std::smatch match;

    if (std::regex_search(lowerWord, match, word_regex))
        return match.str();

    return "";
}