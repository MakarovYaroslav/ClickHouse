#pragma once

#include <Parsers/IAST.h>
#include <DataTypes/IDataType.h>
#include <memory>
#include <unordered_map>

namespace DB
{

struct PreparedSetKey
{
    IAST::Hash ast_hash;
    DataTypes types; /// Empty for subqueries.

    bool operator==(const PreparedSetKey & other) const
    {
        if (ast_hash != other.ast_hash)
            return false;

        if (types.size() != other.types.size())
            return false;

        for (size_t i = 0; i < types.size(); ++i)
        {
            if (!types[i]->equals(*other.types[i]))
                return false;
        }

        return true;
    }

    struct Hash
    {
        UInt64 operator()(const PreparedSetKey & key) const { return key.ast_hash.first; }
    };
};

class Set;
using SetPtr = std::shared_ptr<Set>;

using PreparedSets = std::unordered_map<PreparedSetKey, SetPtr, PreparedSetKey::Hash>;

}
