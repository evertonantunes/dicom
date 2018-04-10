#pragma once

#include <istream>
#include "value_representation.hpp"

namespace dicom
{
    namespace dictionary
    {
        void init_dictionary( std::istream &stream );

        ValueRepresentation get_default_vr( const std::uint16_t group, const std::uint16_t element );
    }
}
