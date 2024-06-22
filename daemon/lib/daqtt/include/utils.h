#pragma once

#include <algorithm>

namespace daqtt {
namespace util {

    template <class T, class Container>
    bool contains(const Container& container, const T& element)
    {
        return std::find(container.begin(), container.end(), element) != container.end();
    }
}
}
