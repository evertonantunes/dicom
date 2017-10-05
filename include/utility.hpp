#pragma once

#include <iostream>
#include <stdexcept>

#define Debug(TXT) std::cout << #TXT << ": " << TXT << std::endl;
#define DUMP(TXT) #TXT << ": " << TXT << "  "
#define DebugTag(group, element) std::cout << "(" << "0x" << std::hex << std::setfill('0') << std::setw(4) << group << ", 0x" << std::hex << std::setfill('0') << std::setw(4) << element << ")" << std::endl;

namespace utility
{
    enum class Endian
    {
          Litle
        , Big

#if __BYTE_ORDER == __LITTLE_ENDIAN
        , platform_endian = Endian::Litle
#else
        , platform_endian = Endian::Big
#endif

    };

    std::ostream& operator<<( std::ostream &out, const Endian endian );

    template<typename T>
    inline T swap_endian( const T value )
    {
        if (sizeof(T) == sizeof(std::uint16_t))
        {
            return static_cast<T>(__builtin_bswap16(static_cast<std::uint16_t>(value)));
        }

        if (sizeof(T) == sizeof(std::uint32_t))
        {
            return static_cast<T>(__builtin_bswap32(static_cast<std::uint32_t>(value)));
        }

        if (sizeof(T) == sizeof(std::uint64_t))
        {
            return static_cast<T>(__builtin_bswap64(static_cast<std::uint64_t>(value)));
        }
    }

    template<typename T>
    T get_value( std::istream &stream, const Endian source_endian = Endian::platform_endian )
    {
        T result;
        stream.read( reinterpret_cast<char*>(&result), sizeof(T) );
        if (stream.gcount() != sizeof(T))
        {
            throw std::runtime_error("Bad stream");
        }

        if ( source_endian != Endian::platform_endian )
        {
            return swap_endian(result);
        }
        return result;
    }

    template<typename T>
    void set_value( std::istream &stream, const T value, const Endian output_endian = Endian::platform_endian )
    {

    }
}


namespace uid
{
    class Uuid
    {
        friend std::ostream& operator<<( std::ostream &out, const Uuid &uuid );

        std::string m_data;
    public:
        Uuid()
        { }

        Uuid( std::string &&data )
            : m_data(std::move(data))
        { }

        inline bool is_valid() const
        {
            return !m_data.empty();
        }

        explicit operator bool() const
        {
            return is_valid();
        }

        bool operator ==( const Uuid &other ) const
        {
            return m_data == other.m_data;
        }
    };

    inline std::ostream& operator<<( std::ostream &out, const Uuid &uuid )
    {
        return out << "UUID(" << uuid.m_data << ")";
    }
}
