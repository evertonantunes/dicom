#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "parser.hpp"
#include "dicom_unique_identifiers.hpp"
#include "dicom_dictionary.hpp"
#include <fstream>

TEST_CASE( "DICOM magic number", "DICM" )
{
    const std::string buffer = {  '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
                                , '0', '0', '0', '0', '0', '0', '0', '0'
                                , 'D', 'I', 'C', 'M'};

    REQUIRE( buffer.size() == (128 + 4) );
    REQUIRE( buffer[128 + 0] == 'D' );
    REQUIRE( buffer[128 + 1] == 'I' );
    REQUIRE( buffer[128 + 2] == 'C' );
    REQUIRE( buffer[128 + 3] == 'M' );

    std::stringstream stream(buffer);

    REQUIRE( dicom::file::is_dicom(stream) );
}

TEST_CASE( "comparing UUIDs equality", "" )
{
    REQUIRE(uid::Uuid("12345") == uid::Uuid("12345"));
}

TEST_CASE( "comparing UUIDs diference", "" )
{
    REQUIRE_FALSE(uid::Uuid("ABCDE") == uid::Uuid("12345"));
}

TEST_CASE( "comparing UUID and string diference", "" )
{
    REQUIRE(uid::Uuid("ABCDE") != "12345");
}

TEST_CASE( "comparing UUID and string equality", "" )
{
    REQUIRE(uid::Uuid("ABCDE") == "12345");
}

TEST_CASE( "Read file", "" )
{
    {
        std::ifstream stream( RESOURCE_DIR "/dicom-dict.txt" );
        dicom::dictionary::init_dictionary( stream );
    }

    std::ifstream stream( SAMPLES_DIR "/anatomic-2k/TG18-CH/TG18-CH-2k-01.dcm" );
    REQUIRE( dicom::file::is_dicom(stream) );

    dicom::DataSet data_set;

    dicom::file::read(stream, data_set, utility::Endian::Litle, true);

    REQUIRE( data_set.size() > 1 );

    REQUIRE( dicom::file::get<dicom::tag::PixelData>(data_set)->value().size() > 0 );

    for ( const dicom::Tag tag: data_set )
    {
        Debug(tag->vr());
    }

}

