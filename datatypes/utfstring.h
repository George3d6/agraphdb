#include <string>

namespace data {
class UTFString {
public:
    //members
    const std::string content;

    //mandatory methods
    std::string serialize() const noexcept {
        return content;
    }

    //Lifetime management
    template <class StringType>
    UTFString(StringType string) noexcept : content(string) {}
    //Mandatory:
    UTFString(std::string serialization) noexcept : content((serialization)) {}
};
}
