#  LRU File Metadata Cache (Linux)

This assignment provides a C++17 library that implements a Least Recently Used (LRU) cache system to  store metadata of files accessed by applications. 
It is specifically designed for **Linux** platforms using the `stat()` system call to fetch file metadata
file e.g size, timestamp, inode(Information returned by stat() system call)
---

## Problem Statement

Create a library that:

- Maintains an **LRU cache** for files accessed by applications
- Stores metadata such as:
  - File path
  - Timestamp of access
  - File size, inode, and other details from `stat()` system call
- Supports removal of entries that have:
  - Not been accessed recently (LRU policy)
  - Exceeded a specified **maximum age** in seconds
- Provides APIs to:
  - Create a new LRU cache
  - Add a file entry
  - Search an entry by filepath
  - Remove a specific entry
  - Evict old entries exceeding the TTL (time-to-live)

---

## Build Instructions

### Prerequisites

- Linux (Ubuntu/Debian/Arch etc.)
- GCC or Clang (C++17)
- CMake ≥ 3.10

## Output Console

/workspaces/lru-file-cache/build/lru_file_cache
@RoyalTejShinde1993 ➜ /workspaces/lru-file-cache/build (main) $ /workspaces/lru-file-cache/build/lru_file_cache

Accessed: /bin/ls   

Size: 142144    

Inode: 1048918  

@RoyalTejShinde1993 ➜ /workspaces/lru-file-cache/build (main) $ 

### Steps

```bash
git clone <your-repo-url>
cd lru_file_cache

mkdir build && cd build
cmake ..
make
./lru_file_cache
