#pragma once

#include <ostream>
#include <string>
#include "utility.hpp"

namespace uid
{
    class Uuid
    {
        friend std::ostream& operator<<( std::ostream &out, const Uuid &uuid );

        std::string m_data;
    public:
        Uuid();

        Uuid( const std::string &data );

        Uuid( std::string &&data );

        inline bool is_valid() const
        {
            return !m_data.empty();
        }

        explicit operator bool() const
        {
            return is_valid();
        }

        inline bool operator ==( const Uuid &other ) const
        {
            return m_data == other.m_data;
        }

        inline bool operator !=( const Uuid &other ) const
        {
            return m_data != other.m_data;
        }

        inline bool operator ==( const std::string &str ) const
        {
            return m_data == str;
        }

        inline bool operator !=( const std::string &str ) const
        {
            return m_data != str;
        }
    };

    std::ostream& operator<<( std::ostream &out, const Uuid &uuid );
}
