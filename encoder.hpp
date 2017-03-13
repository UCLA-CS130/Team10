#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <string>

class Encoder 
{
    public:
        static void encode(std::string &body, std::pair<std::string, std::string> &content_encoding_header, std::string accepted_encodings);

    private:
        enum EncodingType
        {
            IDENTITY,
            GZIP,
            DEFLATE
        };

        static EncodingType choose_encoding(const std::string& accepted_encodings);
        static void gzip_compress(std::string &str);
        static void deflate_compress(std::string &str);
        static void trim(std::string& str);
};

#endif