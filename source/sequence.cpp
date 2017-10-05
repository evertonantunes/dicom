#include "sequence.hpp"
#include "parser.hpp"

dicom::type::Sequence::Sequence(const uint16_t group, const uint16_t element)
    : Abstract(group, element, ValueRepresentation::SQ)
{ }

void dicom::type::Sequence::read(std::istream& stream, const std::ptrdiff_t size, const utility::Endian endian, const bool explicit_vr)
{
    std::size_t index = 0;
    const std::size_t start_pos = stream.tellg();
    while (!stream.eof() && (stream.tellg() < start_pos + size))
    {
        const auto group = utility::get_value<std::uint16_t>(stream);
        const auto element = utility::get_value<std::uint16_t>(stream);

        if (group == 0xFFFE && element == 0xE0DD)
        {
            const auto item_size = utility::get_value<std::int32_t>(stream);
            if (item_size != 0)
            {
                throw std::runtime_error("Bad sequence item delimitation");
            }
            return;
        }

        if (group == 0xFFFE && element == 0xE00D)
        {
            const auto item_size = utility::get_value<std::int32_t>(stream);
            if (item_size != 0)
            {
                throw std::runtime_error("Bad sequence item delimitation");
            }
            return;
        }

        const auto item_size = utility::get_value<std::int32_t>(stream);

        std::vector<Abstract*> data;
        dicom::file::read(stream, data, endian, explicit_vr, item_size);

        for ( auto item: data )
        {
            m_data.push_back({index, item});
        }

        index++;
    }

    if (size != 0)
    {
        const std::size_t value = static_cast<std::size_t>(stream.tellg()) - start_pos;
        if (value != size)
        {
            throw std::runtime_error("bad read sequence size");
        }
    }
}

void dicom::type::Sequence::write(std::istream& stream)
{
}
