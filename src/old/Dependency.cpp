#include "Dependency.h"

Dependency::Dependency(IPackage& from, IPackage& to)
: m_From(&from), m_To(&to)
{
}

