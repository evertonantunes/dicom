#pragma once

#include <vector>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <array>
#include "utility.hpp"
#include "value_representation.hpp"
#include <sstream>

namespace dicom
{
    namespace type
    {
        class Abstract
        {
            std::uint16_t m_group;
            std::uint16_t m_element;
            ValueRepresentation m_vr;
        public:
            Abstract( const std::uint16_t group, std::uint16_t element, ValueRepresentation vr )
                : m_group(group)
                , m_element(element)
                , m_vr(vr)
            { }

            virtual ~Abstract()
            { }

            inline std::uint16_t group() const
            {
                return m_group;
            }

            inline std::uint16_t element() const
            {
                return m_element;
            }

            inline ValueRepresentation vr() const
            {
                return m_vr;
            }

            virtual void read( std::istream &, const std::ptrdiff_t size, const utility::Endian endian, const bool explicit_vr ) = 0;
            virtual void write( std::istream & ) = 0;
        };

        template<typename T, ValueRepresentation VR>
        class BinaryFixedSyze : public Abstract
        {
            std::vector<T> m_value;
        public:

            BinaryFixedSyze( const std::uint16_t group, const std::uint16_t element )
                : Abstract(group, element, VR)
            { }

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian endian, const bool )
            {
                if ( (size % sizeof(T)) != 0 )// check multiplicity
                {
                    throw std::runtime_error("Tag binary of fixed size not contains multiplicity bytes for type.");
                }

                m_value.reserve( size / sizeof(T) );
                for ( std::size_t index = 0; index < size; index += sizeof(T) )
                {
                    m_value.push_back(utility::get_value<T>(stream, endian));
                }
            }

            virtual void write( std::istream &stream )
            {

            }

            inline const std::vector<T>& value() const
            {
                return m_value;
            }
        };

        class Raw : public Abstract
        {
            std::vector<char> m_value;
        public:
            Raw( const std::uint16_t group, const std::uint16_t element, const ValueRepresentation vr )
                : Abstract(group, element, vr)
            { }

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian , const bool )
            {
                m_value.resize( size );
                stream.read( &m_value[0], size );
                if (stream.gcount() != size)
                {
                    throw std::runtime_error("Bad read size");
                }
            }

            virtual void write( std::istream &stream )
            {

            }

            inline const std::vector<char>& value() const
            {
                return m_value;
            }
        };

        class PixelData : public Abstract
        {
            std::vector<std::vector<char>> m_data;
        public:
            PixelData( const std::uint16_t group, const std::uint16_t element, const ValueRepresentation vr )
                : Abstract(group, element, vr)
            { }

            void read_sequence( std::istream &stream, const utility::Endian endian )
            {
                std::vector<std::uint32_t> offsets;
                std::vector<std::vector<char>> items;
                while (!stream.eof())
                {
                    const auto group = utility::get_value<std::uint16_t>(stream);
                    const auto element = utility::get_value<std::uint16_t>(stream);

                    if (group == 0xFFFE && element == 0xE0DD)
                    {
                        const auto item_size = utility::get_value<std::int32_t>(stream);
                        if (item_size != 0)
                        {
                            throw std::runtime_error("Bad sequence item delimitation");
                        }
                        break;
                    }

                    if (group == 0xFFFE && element == 0xE00D)
                    {
                        const auto item_size = utility::get_value<std::int32_t>(stream);
                        if (item_size != 0)
                        {
                            throw std::runtime_error("Bad sequence item delimitation");
                        }
                        break;
                    }

                    const auto item_size = utility::get_value<std::int32_t>(stream);
                    if (offsets.empty())
                    {
                        offsets.reserve( item_size / sizeof(std::uint32_t) );
                        for ( std::size_t index = 0; index < item_size; index += sizeof(std::uint32_t) )
                        {
                            offsets.push_back(utility::get_value<std::int32_t>(stream, endian));
                        }
                    }
                    else
                    {
                        std::vector<char> item;
                        item.resize( item_size );
                        stream.read( &item[0], item_size );
                        items.push_back( std::move(item) );
                    }
                }

                if (offsets.size() == 1 && offsets.front() == 0)
                {
                    m_data = std::move(items);
                }
                else
                {
                    throw std::runtime_error("Pixel data not suported");
                }
            }

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian endian, const bool explicit_vr  )
            {
                if (vr() == ValueRepresentation::SQ)
                {
                    read_sequence(stream, endian);
                }
                else
                {
                    if (size == -1)
                    {
                        read_sequence(stream, endian);
                    }
                    else
                    {
                        throw std::runtime_error("Pixel data not suported");
                    }
                }
            }

            virtual void write( std::istream &stream )
            {
            }

            inline const std::vector<std::vector<char>>& value() const
            {
                return m_data;
            }
        };

        template<short SIZE>
        class StringFixedSize : public Abstract
        {
            std::array<char, SIZE> m_value;
        public:
            StringFixedSize( const std::uint16_t group, const std::uint16_t element, const ValueRepresentation vr )
                : Abstract(group, element, vr)
            { }

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian , const bool )
            {
                if (size != m_value.size())
                {
                    throw std::runtime_error("Fixed string invalid size");
                }

                stream.read( &m_value[0], m_value.size() );
                if (stream.gcount() != m_value.size())
                {
                    throw std::runtime_error("Bad read size");
                }
            }

            virtual void write( std::istream &stream )
            {

            }

            inline const std::array<char, SIZE>& value() const
            {
                return m_value;
            }
        };

        class DecimalString : public Abstract
        {
            std::vector<double> m_value;
        public:
            DecimalString( const std::uint16_t group, const std::uint16_t element, const ValueRepresentation vr )
                : Abstract(group, element, vr)
            { }

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian , const bool )
            {
                std::string raw_data;

                raw_data.resize(size);
                stream.read(&raw_data[0], size);

                if (stream.tellg() != size)
                {
                    throw std::runtime_error("bad decimal string size");
                }

                {
                    std::stringstream raw_stream(raw_data);

                    double value = 0;
                    while (raw_stream >> value)
                    {
                        m_value.push_back(value);
                        raw_stream.get();
                    }
                }
            }

            virtual void write( std::istream &stream )
            {

            }

            inline const std::vector<double>& value() const
            {
                return m_value;
            }
        };

        class UniquelyIdentify : public Abstract
        {
            uid::Uuid m_value;
        public:
            UniquelyIdentify( const std::uint16_t group, const std::uint16_t element, const ValueRepresentation vr )
                : Abstract(group, element, vr)
            { }

            virtual void read( std::istream &stream, const std::ptrdiff_t size, const utility::Endian , const bool )
            {
                std::string raw_data;

                raw_data.resize(size);
                stream.read(&raw_data[0], size);

                raw_data.erase(   std::remove_if(begin(raw_data), end(raw_data), []( const char c ) { return c == '\0'; })
                                  , end(raw_data) );

                m_value = uid::Uuid(std::move(raw_data));

                if (stream.gcount() != size)
                {
                    throw std::runtime_error("Bad uid size");
                }
            }

            virtual void write( std::istream &stream )
            {

            }

            inline const uid::Uuid& value() const
            {
                return m_value;
            }
        };

    }
}
