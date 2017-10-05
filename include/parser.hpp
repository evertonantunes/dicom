#pragma once

#include "value_representation.hpp"
#include "utility.hpp"
#include "tag.hpp"

#include <algorithm>
#include <limits>


namespace dicom
{
    namespace parser
    {
        type::Abstract* create_tag( const std::uint16_t group, const std::uint16_t element, const ValueRepresentation vr, const std::ptrdiff_t size );

        bool is_dicom( std::istream &stream );

        void read( std::istream &stream, std::vector<type::Abstract*> &data, const utility::Endian endian, bool explicit_vr, const std::size_t max = std::numeric_limits<std::uint32_t>::max());

        type::Abstract* get( const std::vector<type::Abstract*> &data_set, const std::uint16_t group, const std::uint16_t element );

        template<typename T>
        inline typename T::type* get( const std::vector<type::Abstract*> &data_set )
        {
            const auto tag = get(data_set, T::group, T::element);
            if (!tag)
            {
                return nullptr;
            }

            if ( const auto tag_typed = dynamic_cast<typename T::type*>(tag) )
            {
                return tag_typed;
            }
            else
            {
                throw std::runtime_error("Bad type cast DICOM tag");
            }
        }
    }

    namespace tag
    {
        template<std::uint16_t G, std::uint16_t E, typename T>
        struct TagType
        {
            constexpr static std::uint16_t group = G;
            constexpr static std::uint16_t element = E;
            typedef T type;
        };

        typedef TagType<0x7FE0, 0x0010, type::PixelData> PixelData;
    }

    namespace file
    {
        using parser::get;
        using parser::read;
        using parser::is_dicom;
    }

    using Tag = type::Abstract*;
    using DataSet = std::vector<Tag>;
}
