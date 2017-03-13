#include "encoder.hpp"
#include "strtk/strtk.hpp"
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

void Encoder::trim(std::string& str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

Encoder::EncodingType Encoder::choose_encoding(const std::string& accepted_encodings) {
    std::cout << "we're not crazy: " << accepted_encodings << std::endl;
    std::string trimmed_accepted_encodings = accepted_encodings;
    trim(trimmed_accepted_encodings);
    std::cout << "Strip whitespace: " << trimmed_accepted_encodings << std::endl;
    std::vector<std::string> encodings;
    strtk::parse(trimmed_accepted_encodings, ",", encodings);

    std::multimap<float, std::string, std::greater<float>> encoding_map;
    for (const auto &encoding: encodings) {
        for (std::size_t i = 0; i < encoding.size(); i++) {
            if (encoding[i] == ';') {
                std::string algo = encoding.substr(0, i);
                float q_value = std::stof(encoding.substr(i + 1));
                encoding_map.insert(std::make_pair(q_value, algo));
            }
        }
    }
    
    std::string encoding_type_string = "";
    std::multimap<float, std::string, std::greater<float>>::iterator it = encoding_map.begin();
    if (it != encoding_map.end()) {
        encoding_type_string = it->second;
    }
    std::cout << "Encoding type string" << encoding_type_string << std::endl;
    if (encoding_type_string == "gzip") {
        return EncodingType::GZIP;
    }
    return EncodingType::IDENTITY;
}

bool Encoder::encode(std::string &body, std::pair<std::string, std::string> &content_encoding_header, std::string accepted_encodings) {
    EncodingType encoding_type = choose_encoding(accepted_encodings);
    switch(encoding_type) {
        case GZIP:
            std::cout << "potato" << std::endl;
            gzip_compress(body);
            content_encoding_header.second = "gzip";
            break;
        default:
            break;
    }
    return true;
}

bool Encoder::gzip_compress(std::string& str)
{
    boost::iostreams::gzip_params params;
    params.level = boost::iostreams::gzip::best_compression;

    std::stringstream result;
    boost::iostreams::filtering_ostream stream;
    stream.push(boost::iostreams::gzip_compressor());
    stream.push(result);
    stream << str;
    boost::iostreams::close(stream);
    str = result.str();
    return true;
}
