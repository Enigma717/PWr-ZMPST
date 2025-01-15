#pragma once

#include <cstdint>
#include <string>

struct ModelParams {
    std::size_t vertices {0};
    std::size_t edges {0};
    std::string paths_file {""};
    std::string demands_dir {""};
};
