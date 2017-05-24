#include <string>

namespace data {
class Float64 {
public:
    //members
    const double content;

    //mandatory methods
    std::string serialize() const noexcept {
        return std::to_string(content);
    }

    //Lifetime management
    template <class NumberType>
    Float64(NumberType number) noexcept : content(number) {}
    //Mandatory:
    Float64(std::string serialization) noexcept : content(std::stod(serialization)) {}
};
}
