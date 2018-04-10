#include "parser.hpp"
#include "dicom_dictionary.hpp"
#include "sequence.hpp"
#include "utility.hpp"
#include "dicom_unique_identifiers.hpp"

bool dicom::parser::is_dicom( std::istream &stream )
{
    char buffer[128] = {};
    char magic[4] = {};
    stream.read(&buffer[0], sizeof(buffer));

    if (stream.gcount() != sizeof(buffer))
    {
        return false;
    }

    stream.read(&magic[0], sizeof(magic));
    if (stream.gcount() != sizeof(magic))
    {
        return false;
    }

    return magic[0] == 'D' && magic[1] == 'I' && magic[2] == 'C' && magic[3] == 'M';
}

void dicom::parser::read(std::istream& stream, std::vector<dicom::type::Abstract*>& data, const utility::Endian endian, bool explicit_vr, const std::size_t max)
{
    stream.peek();
    const std::size_t start_pos = stream.tellg();
    while ((!stream.eof()) && (stream.tellg() < start_pos + max))
    {
        const auto group = utility::get_value<std::uint16_t>(stream);
        const auto element = utility::get_value<std::uint16_t>(stream);

        std::ptrdiff_t size = 0;

        ValueRepresentation vr = static_cast<ValueRepresentation>(0);

        if (explicit_vr)
        {
            vr = static_cast<ValueRepresentation>(utility::get_value<std::uint16_t>(stream));
            Debug(ValueRepresentation::DA);
            Debug(vr);

            if (!is_valid_vr(vr))
            {
                throw std::runtime_error("Bad file VR");
            }

            switch (vr)
            {
                case ValueRepresentation::OB:
                case ValueRepresentation::OW:
                case ValueRepresentation::OF:
                case ValueRepresentation::SQ:
                case ValueRepresentation::UT:
                case ValueRepresentation::UN:
                {
                    const auto reserved = utility::get_value<std::uint16_t>(stream);
                    size = utility::get_value<std::int32_t>(stream);
                }
                break;
                default:
                    size = utility::get_value<std::int16_t>(stream);
                break;
            }
        }
        else
        {
            vr = dicom::dictionary::get_default_vr(group, element);

            switch (vr)
            {
                case ValueRepresentation::SQ:
                    size = utility::get_value<std::int32_t>(stream);
                break;
                default:
                    size = utility::get_value<std::int16_t>(stream);
                break;
            }
        }

        auto tag = create_tag(group, element, vr, size);
        tag->read(stream, size, endian, explicit_vr);
        data.push_back(tag);

        if (group == 0x0002 && element == 0x0010) // TransferSyntaxUID
        {
            if ( const auto transfer_syntax_uid = dynamic_cast<const type::UniquelyIdentify*>(tag) )
            {
                if (dicom::uids::ImplicitVRLittleEndian == transfer_syntax_uid->value())
                {
                    explicit_vr = false;
                }
            }
        }

        stream.peek();
    }
}

dicom::type::Abstract*dicom::parser::get(const std::vector<dicom::type::Abstract*>& data_set, const uint16_t group, const uint16_t element)
{
    const auto item = std::find_if(begin(data_set), end(data_set), [group, element]( const type::Abstract *item )
    {
        return item->group() == group && item->element() == element;
    });
    if (item != end(data_set))
    {
        return *item;
    }
    return nullptr;
}

dicom::type::Abstract*dicom::parser::create_tag(const uint16_t group, const uint16_t element, const dicom::ValueRepresentation vr, const std::ptrdiff_t size)
{
    switch (vr)
    {
        case ValueRepresentation::SQ:
            return new type::Sequence(group, element);
        case ValueRepresentation::UL:
            return new type::BinaryFixedSyze<std::uint32_t, ValueRepresentation::UL>(group, element);
        case ValueRepresentation::US:
            return new type::BinaryFixedSyze<std::uint16_t, ValueRepresentation::US>(group, element);
        case ValueRepresentation::SS:
            return new type::BinaryFixedSyze<std::int16_t, ValueRepresentation::SS>(group, element);
        case ValueRepresentation::SL:
            return new type::BinaryFixedSyze<std::int32_t, ValueRepresentation::SL>(group, element);
        case ValueRepresentation::OF:
            return new type::BinaryFixedSyze<std::float_t, ValueRepresentation::OF>(group, element);
        case ValueRepresentation::OW:
            return new type::BinaryFixedSyze<std::int16_t, ValueRepresentation::OW>(group, element);
        case ValueRepresentation::OD:
            return new type::BinaryFixedSyze<double, ValueRepresentation::OD>(group, element);
        case ValueRepresentation::FD:
            return new type::BinaryFixedSyze<double, ValueRepresentation::FD>(group, element);
        case ValueRepresentation::FL:
            return new type::BinaryFixedSyze<float, ValueRepresentation::FL>(group, element);
        case ValueRepresentation::UI:
            return new type::UniquelyIdentify(group, element, vr);
        case ValueRepresentation::AS:
        case ValueRepresentation::AT:
        {
            if (size == 4)
            {
                return new type::StringFixedSize<4>(group, element, vr);
            }
        }
        case ValueRepresentation::DA:
        {
            if (size == 8)
            {
                return new type::StringFixedSize<8>(group, element, vr);
            }
        }
        case ValueRepresentation::OB:
        {
            if (group == 0x7FE0 && element == 0x0010)
            {
                return new type::PixelData(group, element, vr);
            }
        }
        case ValueRepresentation::UT:
        case ValueRepresentation::UN:
        default:
            return new type::Raw(group, element, vr);
    }
}
