#pragma once

#include <cstdint>
#include <string>

class Model;

class Loader {
public:
    Loader() = delete;
    Loader(Model& model_ref);

    void parse_instance(const std::string& file_path);

private:
    void parse_header(const std::string& read_line);
    void parse_distances_matrix(const std::string& read_line);

    Model& model_ref;
    std::size_t currently_read_line {0};
};
