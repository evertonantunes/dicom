#pragma once

#include "value_representation.hpp"

namespace dicom
{
    namespace dictionary
    {
        ValueRepresentation get_default_vr( const std::uint16_t group, const std::uint16_t element );
    }
}
