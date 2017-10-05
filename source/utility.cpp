#include "utility.hpp"

std::ostream& utility::operator<<( std::ostream &out, const utility::Endian endian )
{
    if (endian == utility::Endian::Litle)
    {
        return out << "Litle";
    }
    return out << "Big";
}

