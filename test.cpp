#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{6, 4, 9};
    for(auto it = std::begin(v)+1; it != std::end(v); ++it) {
        std::cout << *it << "\n";
    }
}