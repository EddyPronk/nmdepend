#include "Entity.h"

Entity::Entity(const Name_t& name, Entity* owner) : m_Name(name), m_Owner(owner)
{
}

std::string Entity::Name() const
{
   return m_Name;
}


