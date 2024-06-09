#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#define UNIT_LITERAL(UNIT) \
inline layout_value operator ""_##UNIT (unsigned long long v) { \
    return layout_value{v, layout_value_unit::UNIT}; \
}

enum class layout_value_unit {
    fr, px
};


struct layout_value {
    unsigned long long v;
    layout_value_unit unit;
};


UNIT_LITERAL(fr)
UNIT_LITERAL(px)


struct reagon {
    unsigned int x1, y1, x2, y2;
};

class layout
{
    std::vector<layout_value> horizontal;
    std::vector<layout_value> vertical;
    std::unordered_map<std::string, reagon> reagons;


    double calculate_fr_factor(std::vector<layout_value>& layout, unsigned long long pixel_length);

public:
    layout(const std::vector<layout_value>& horizontal, const std::vector<layout_value>& vertical, const std::initializer_list<std::initializer_list<std::string>>& layout);

    void set_reagon(const std::string& name);
};

// auto l = layout{
//     {10_px, 1_fr}, 
//     {10_px, 1_fr},
//     {
//         {"close", "blank"},
//         {"main", "main"}
//     }
// };