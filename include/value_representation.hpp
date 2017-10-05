#pragma once

#include <ostream>
#include <vector>

namespace dicom
{
    namespace impl
    {
        constexpr std::uint16_t pack( const char *value )
        {
            return (static_cast<std::uint16_t>(value[1]) << 8) | value[0];
        }
    }

#define DICOM_VR_LIST \
        DICOM_VR_LIST_MAKE(AE) \
        DICOM_VR_LIST_MAKE(AS) \
        DICOM_VR_LIST_MAKE(AT) \
        DICOM_VR_LIST_MAKE(OB) \
        DICOM_VR_LIST_MAKE(OW) \
        DICOM_VR_LIST_MAKE(OF) \
        DICOM_VR_LIST_MAKE(OD) \
        DICOM_VR_LIST_MAKE(SQ) \
        DICOM_VR_LIST_MAKE(UT) \
        DICOM_VR_LIST_MAKE(UN) \
        DICOM_VR_LIST_MAKE(UL) \
        DICOM_VR_LIST_MAKE(US) \
        DICOM_VR_LIST_MAKE(SS) \
        DICOM_VR_LIST_MAKE(SL) \
        DICOM_VR_LIST_MAKE(FD) \
        DICOM_VR_LIST_MAKE(FL) \
        DICOM_VR_LIST_MAKE(DA) \
        DICOM_VR_LIST_MAKE(UI)


    enum class ValueRepresentation : std::uint16_t
    {
#define DICOM_VR_LIST_MAKE(ITEM) ITEM = impl::pack(#ITEM),
        DICOM_VR_LIST
#undef DICOM_VR_LIST_MAKE
    };

    static_assert(sizeof(ValueRepresentation) == 2, "DICOM Value representation require 16bits aligin");

    std::ostream& operator<<( std::ostream &out, const ValueRepresentation vr );

    bool is_valid_vr( const ValueRepresentation vr );

    std::vector<ValueRepresentation> get_value_representations_list();
}
