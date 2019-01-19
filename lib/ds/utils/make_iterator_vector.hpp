
#pragma once

#include <list>
#include <vector>

template <typename Ctnr>
std::vector<typename Ctnr::iterator> make_iterator_vector(Ctnr& c) {
    using std::size;
    using std::begin;
    using std::end;
    std::vector<typename Ctnr::iterator> res;
    res.reserve(size(c));
    for (auto it = begin(c); it != end(c); ++it) {
        res.push_back(it);
    }
    return res;
}
