#include "HashMap.hpp"
#include <string>

HashMap::HashMap()
{
}

void add(const std::string& key, const std::string& value)
{
    int val;
    val = computeHash(key);

}

int computeHash(std::string const & key)
{
    int hashVal;
    int len;
    
    for (int i = 0; i < key.length(); ++i)
    {
        hashVal += key[i]*71;
    }

    return hashVal;

}
