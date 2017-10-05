#include "value_representation.hpp"

std::ostream& dicom::operator<<(std::ostream& out, const dicom::ValueRepresentation vr)
{
    return out << reinterpret_cast<const char*>(&vr)[0]
               << reinterpret_cast<const char*>(&vr)[1];
}

bool dicom::is_valid_vr(const dicom::ValueRepresentation vr)
{
#define DICOM_VR_LIST_MAKE(ITEM) case dicom::ValueRepresentation::ITEM: return true;
    switch (vr)
    {
        DICOM_VR_LIST;
        default:
            return false;
        break;
    }
#undef DICOM_VR_LIST_MAKE
}

std::vector<dicom::ValueRepresentation> dicom::get_value_representations_list()
{
#define DICOM_VR_LIST_MAKE(ITEM) dicom::ValueRepresentation::ITEM,
    return
    {
        DICOM_VR_LIST
    };
#undef DICOM_VR_LIST_MAKE
}
