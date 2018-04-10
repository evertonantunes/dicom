#include "uid.hpp"

uid::Uuid::Uuid()
{ }

uid::Uuid::Uuid(const std::string& data)
    : m_data(data)
{ }

uid::Uuid::Uuid( std::string &&data )
    : m_data(std::move(data))
{ }

std::ostream&uid::operator<<(std::ostream& out, const uid::Uuid& uuid)
{
    return out << "UUID(" << uuid.m_data << ")";
}
