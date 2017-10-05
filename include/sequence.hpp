#pragma once
#pragma once

#include <vector>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <algorithm>
#include <array>
#include "tag.hpp"

namespace dicom
{
    namespace type
    {
        class Sequence : public Abstract
        {
            std::vector<std::pair<std::size_t, Abstract*>> m_data;
        public:
            Sequence( const std::uint16_t group, const std::uint16_t element );

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian endian, const bool explicit_vr  );

            virtual void write( std::istream &stream );

            inline const std::vector<std::pair<std::size_t, Abstract*>>& value() const
            {
                return m_data;
            }
        };
    }
}
