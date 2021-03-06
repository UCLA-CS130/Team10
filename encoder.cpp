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
    std::string trimmed_accepted_encodings = accepted_encodings;
    trim(trimmed_accepted_encodings);
    std::vector<std::string> encodings;
    strtk::parse(trimmed_accepted_encodings, ",", encodings);

    std::multimap<float, std::string, std::greater<float>> encoding_map;
    for (const auto &encoding: encodings) {
        for (std::size_t i = 0; i < encoding.size(); i++) {
            if (encoding[i] == ';') {
                std::string algo = encoding.substr(0, i);
                std::string q_value_string = encoding.substr(i + 3);
                float q_value = std::stof(q_value_string);
                encoding_map.insert(std::make_pair(q_value, algo));
                break;
            }
            if (i == encoding.size() - 1) {
                encoding_map.insert(std::make_pair(1.0, encoding));
            }
        }            
    }
    
    std::string encoding_type_string = "";
    std::multimap<float, std::string, std::greater<float>>::iterator it = encoding_map.begin();
    if (it != encoding_map.end()) {
        encoding_type_string = it->second;
    }

    if (encoding_type_string == "gzip") {
        return EncodingType::GZIP;
    } else if (encoding_type_string == "deflate") {
        return EncodingType::DEFLATE;
    }
    return EncodingType::IDENTITY;
}

void Encoder::encode(std::string &body, std::pair<std::string, std::string> &content_encoding_header, std::string accepted_encodings) {
    EncodingType encoding_type = choose_encoding(accepted_encodings);
    switch(encoding_type) {
        case GZIP:
            gzip_compress(body);
            content_encoding_header.second = "gzip";
            break;
        case DEFLATE:
            deflate_compress(body);
            content_encoding_header.second = "deflate";
            break;
        default:
            break;
    }
}

void Encoder::gzip_compress(std::string& str)
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
}

void Encoder::deflate_compress(std::string& str)
{
    boost::iostreams::zlib_params params;
    params.level = boost::iostreams::zlib::best_compression;
    params.noheader = true;

    std::stringstream result;
    boost::iostreams::filtering_ostream stream;
    stream.push(boost::iostreams::zlib_compressor(params));
    stream.push(result);
    stream << str;
    boost::iostreams::close(stream);
    str = result.str();
}