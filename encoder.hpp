#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <string>

class Encoder 
{
    public:
        bool encode(std::string &body, std::pair<std::string, std::string> &content_encoding_header, std::string accepted_encodings);

    private:
        enum EncodingType
        {
            IDENTITY,
            GZIP,
            DEFLATE
        };

        EncodingType choose_encoding(const std::string& accepted_encodings);
        bool gzip_compress(std::string &str);
        bool deflate_compress(std::string &str);
        void trim(std::string& str);
};

#endif