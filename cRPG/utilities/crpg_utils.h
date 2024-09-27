#pragma once
#include<vector>

// Template function to remove an element from a vector at a specific index.
// 'T' represents the type of elements stored in the vector.
template <typename T>
void remove(std::vector<T>& vec, size_t index)
{
    // Check if the index is within the valid range (0 <= index < vec.size()).
    if (index >= vec.size()) {
        // If the index is out of bounds, do nothing or you may throw an exception.
        return;
    }

    // The erase function removes an element at the specified position.
    // vec.begin() gives an iterator to the first element of the vector.
    // Adding 'index' to vec.begin() advances the iterator to the element at 'index'.
    vec.erase(vec.begin() + index);

    // After erasing, all elements after the removed one shift to the left,
    // and the vector size decreases by one.
}