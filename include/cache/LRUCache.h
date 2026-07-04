#pragma once

#include <list>
#include <unordered_map>
#include <mutex>

template <typename Key, typename Value>
class LRUCache
{
private:

    int capacity;

    std::list<std::pair<Key, Value>> items;

    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> lookup;

    std::mutex cacheMutex;

public:

    LRUCache(int capacity)
        : capacity(capacity)
    {}

    bool get(const Key& key, Value& value)
    {
        std::lock_guard<std::mutex> lock(cacheMutex);

        auto it = lookup.find(key);

        if(it == lookup.end())
        {
            return false;
        }

        items.splice(items.begin(), items, it->second);

        value = it->second->second;

        return true;
    }

    void put(const Key& key, const Value& value)
    {
        std::lock_guard<std::mutex> lock(cacheMutex);

        auto it = lookup.find(key);

        if(it != lookup.end())
        {
            it->second->second = value;

            items.splice(items.begin(), items, it->second);

            return;
        }

        items.push_front({key, value});

        lookup[key] = items.begin();

        if(static_cast<int>(items.size()) > capacity)
        {
            auto last = items.back();

            lookup.erase(last.first);

            items.pop_back();
        }
    }

    void remove(const Key& key)
    {
        std::lock_guard<std::mutex> lock(cacheMutex);

        auto it = lookup.find(key);

        if(it != lookup.end())
        {
            items.erase(it->second);

            lookup.erase(it);
        }
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(cacheMutex);

        items.clear();

        lookup.clear();
    }
};
