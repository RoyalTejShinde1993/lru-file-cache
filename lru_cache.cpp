#include <unordered_map>
#include <list>
#include <string>
#include <ctime>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <unistd.h>

struct FileMetadata {
    std::string path;
    std::time_t access_time;
    struct stat statbuf;
};

class LRUCache {
public:
    LRUCache(size_t capacity, time_t max_age_sec)
        : capacity_(capacity), max_age_(max_age_sec) {}

    void put(const std::string& filepath) {
        auto now = std::time(nullptr);

        struct stat st;
        if (stat(filepath.c_str(), &st) != 0) {
            std::cerr << "Error: Cannot stat file " << filepath << std::endl;
            return;
        }

        if (cache_.count(filepath)) {
            cache_[filepath].first->access_time = now;
            cache_[filepath].first->statbuf = st;
            moveToFront(filepath);
        } else {
            if (cache_.size() >= capacity_) {
                std::string least_used = lru_list_.back();
                lru_list_.pop_back();
                cache_.erase(least_used);
            }

            lru_list_.push_front(filepath);
            auto metadata = std::make_shared<FileMetadata>(FileMetadata{filepath, now, st});
            cache_[filepath] = {metadata, lru_list_.begin()};
        }
    }

    std::shared_ptr<FileMetadata> get(const std::string& filepath) {
        if (cache_.count(filepath)) {
            cache_[filepath].first->access_time = std::time(nullptr);
            moveToFront(filepath);
            return cache_[filepath].first;
        }
        return nullptr;
    }

    void remove(const std::string& filepath) {
        if (cache_.count(filepath)) {
            lru_list_.erase(cache_[filepath].second);
            cache_.erase(filepath);
        }
    }

    void evictExpired() {
        auto now = std::time(nullptr);
        for (auto it = lru_list_.rbegin(); it != lru_list_.rend(); ) {
            const auto& key = *it;
            auto metadata = cache_[key].first;
            if ((now - metadata->access_time) > max_age_) {
                auto to_erase = std::next(it).base();
                cache_.erase(key);
                lru_list_.erase(to_erase);
                it = std::make_reverse_iterator(to_erase);
            } else {
                ++it;
            }
        }
    }

private:
    using ListIterator = std::list<std::string>::iterator;

    void moveToFront(const std::string& key) {
        lru_list_.erase(cache_[key].second);
        lru_list_.push_front(key);
        cache_[key].second = lru_list_.begin();
    }

    size_t capacity_;
    time_t max_age_;
    std::list<std::string> lru_list_;
    std::unordered_map<std::string, std::pair<std::shared_ptr<FileMetadata>, ListIterator>> cache_;
};

int main() {
    LRUCache cache(3, 60); // Max 3 files, expire after 60 seconds

    std::string file1 = "/bin/ls";
    std::string file2 = "/bin/bash";

    cache.put(file1);
    cache.put(file2);

    auto meta = cache.get(file1);
    if (meta) {
        std::cout << "Accessed: " << meta->path
                  << "\nSize: " << meta->statbuf.st_size
                  << "\nInode: " << meta->statbuf.st_ino << "\n";
    }

    cache.evictExpired();
    cache.remove(file2);

    return 0;
}
