#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity
{
public:
   typedef std::string Name_t;
   Entity(const Name_t& name, Entity* owner = 0);
   std::string Name() const;
   Name_t m_Name;
   Entity* m_Owner;
private:
   Entity();
};

#endif

