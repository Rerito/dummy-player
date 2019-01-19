
#pragma once

#include <list>
#include <functional>
#include <vector>

template <typename Ctnr>
std::vector<std::reference_wrapper<typename Ctnr::value_type> > make_ref_wrapper_vector(Ctnr& c) {
    using std::size;
    using std::begin;
    using std::end;
    std::vector<typename Ctnr::iterator> res;
    res.reserve(size(c));
    for (auto it = begin(c); it != end(c); ++it) {
        res.push_back(*it);
    }
    return res;
}
