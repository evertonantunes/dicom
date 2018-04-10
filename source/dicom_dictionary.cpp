#include "dicom_dictionary.hpp"
#include <sstream>
#include "utility.hpp"
#include <algorithm>
#include <vector>

namespace
{
    // trim from start (in place)
    inline void ltrim( std::string &s )
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
        {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
        {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    inline std::string trim( std::string s )
    {
        ltrim(s);
        rtrim(s);
        return s;
    }

    static std::vector<std::pair<std::pair<std::uint16_t, std::uint16_t>, dicom::ValueRepresentation>> s_dictionary;
}

void dicom::dictionary::init_dictionary( std::istream &stream )
{
    for ( std::string line; std::getline(stream, line); )
    {
        if (line[0] != '{')
        {
            continue;
        }

        const std::size_t start = 1;
        const std::size_t end_group_pos = line.find_first_of(',', start) + 1;
        const std::size_t end_element_pos = line.find_first_of(',', end_group_pos) + 1;
        const std::size_t start_vr_pos = line.find_first_of('\'', end_element_pos) + 1;
        const std::size_t end_vr_pos = line.find_first_of('\'', start_vr_pos);

        const auto str_group = trim(line.substr(start, end_group_pos - start));
        const auto str_element = trim(line.substr(end_group_pos, end_element_pos - end_group_pos));
        const auto str_vr = line.substr(start_vr_pos, end_vr_pos - start_vr_pos);

        const std::uint16_t group = std::stoi(str_group, 0, 16);
        const std::uint16_t element = std::stoi(str_element, 0, 16);

        if (group == 0 && element == 0)
        {
            continue;
        }

        dicom::ValueRepresentation vr = static_cast<dicom::ValueRepresentation>(0);
        if (str_vr.size() == 2)
        {
            vr = *reinterpret_cast<const dicom::ValueRepresentation*>( str_vr.c_str() );
            if (!is_valid_vr(vr))
            {
                continue;
            }
        }

        // std::cout << DUMP(group) << DUMP(element) << DUMP(vr) << std::endl;

        s_dictionary.push_back( std::make_pair( std::make_pair( group, element ), vr ) );
    }
}


dicom::ValueRepresentation dicom::dictionary::get_default_vr(const uint16_t group, const uint16_t element)
{
    const auto it = std::find_if(begin(s_dictionary), end(s_dictionary), [group, element]( const std::pair<std::pair<std::uint16_t, std::uint16_t>, dicom::ValueRepresentation> &item )
    {
        return item.first.first == group && item.first.second == element;
    });
    if (it != end(s_dictionary))
    {
        return it->second;
    }
    std::cout << DUMP(group) << DUMP(element) << std::endl;
    throw std::runtime_error("Tag not found");
}
