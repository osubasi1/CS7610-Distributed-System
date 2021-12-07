

#include <list>
#include <unordered_map>
#include <mutex>
#include <cstdint>


namespace MaxLRUCache{
    struct Pair {
        public:
        int key;
        int value;

        Pair(int k, int v) : key(std::move(k)), value(std::move(v)) {}
    };

    class Cache {
        private:
            std::unordered_map<int, std::list<Pair>::iterator> cache_;
            std::list<Pair> keys_;
            size_t maxSize_;
            size_t flex_;
            mutable std::mutex lock_;
            using Guard = std::lock_guard<std::mutex>;
        
        protected:
            size_t trim() {
                size_t max_allowed_size = maxSize_ + flex_;
                if(maxSize_ == 0 || cache_.size() < max_allowed_size) {
                    return 0;
                }
                size_t count = 0;
                while (cache_.size() > maxSize_) {
                    cache_.erase(keys_.back().key);
                    keys_.pop_back();
                    count++;
                }
                return count;
        }
        public:
            Cache(const Cache&) = delete;
            Cache& operator = (const Cache&) = delete;


            explicit Cache(size_t maxSize=100, size_t flex=0)
            : maxSize_(maxSize), flex_(flex) {}
            virtual ~Cache() = default;

            size_t size() const {
                Guard guard(lock_);
                return cache_.size();
            }
            bool empty() const {
                Guard g(lock_);
                return cache_.empty();
            }
            void clear() {
                Guard g(lock_);
                cache_.clear();
                keys_.clear();
            }
            void addRecord(int k, int v) {
                Guard g(lock_);
                const auto iter = cache_.find(k);
                if (iter != cache_.end()) {
                    iter->second->value = v;
                    keys_.splice(keys_.begin(), keys_, iter->second);
                    return;
                }
                if (maxSize_ > 0) {
                keys_.emplace_front(k, std::move(v));
                cache_[k] = keys_.begin();
                trim();
                }
            }
            const int& getRecord(const int& k) {
                Guard g(lock_);
                const auto iter = cache_.find(k);
                if (iter == cache_.end()) {
                    // throw std::invalid_argument("key not found");
                    return -1;
                }
                keys_.splice(keys_.begin(), keys_, iter->second);
                return iter->second->value;
            }
            
  
            bool removeRecord(const int& k) {
                Guard g(lock_);
                auto iter = cache_.find(k);
                if (iter == cache_.end()) {
                    return false;
                }
                keys_.erase(iter->second);
                cache_.erase(iter);
                return true;
            }
            bool hasKey (const int& k) const {
                Guard guard(lock_);
                return cache_.find(k) != cache_.end();
            }
            size_t getMaxSize() const { 
                return maxSize_; 
            }
            size_t getFlex() const { 
                return flex_; 
                }
            size_t getMaxAllowedSize() const { 
                return maxSize_ + flex_; 
            }

            void setCacheSize (size_t size) {
                maxSize_ = size;
            }
            std::list<Pair> getPairs(){
                return keys_;
            }
            std::string toString(){
                std::string s;
                for (auto const& i : keys_) {
                    s += std::to_string(i.key) + "\t" + std::to_string(i.value) + "\n";
                }
                if (!s.empty() && s[s.length()-1] == '\n') {
                    s.erase(s.length()-1);
                }
                return s;
            }
    };
}