#include "dicom_dictionary.hpp"

dicom::ValueRepresentation dicom::dictionary::get_default_vr(const uint16_t group, const uint16_t element)
{
    return static_cast<dicom::ValueRepresentation>(0);
}
