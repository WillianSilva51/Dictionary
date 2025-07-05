#include "text_processor/TextProcessor.hpp"

TextProcessor::TextProcessor(const std::string &input_file) : file_stream("files/" + input_file)
{
    if (!file_stream or !file_stream.is_open())
    {
        std::cerr << "ERRO CRÍTICO: Não foi possível abrir o ficheiro de entrada '" << input_file << "'." << std::endl;
        exit(EXIT_FAILURE); // Termina o programa se o ficheiro não puder ser lido.
    }
}

void TextProcessor::toLowerCase(std::string &text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
                   { return std::tolower(c); });
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

    static const std::regex word_regex("[a-zà-ÿ]+(?:[-'][a-zà-ÿ]+)*");

    if (std::regex_match(lowerWord, word_regex))
        return lowerWord;

    return "";
}