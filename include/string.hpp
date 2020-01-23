// string.hpp

#pragma once

#include <string_view>
#include <vector>

namespace extra {

class strtok {
public:
    template<class... Args>
    constexpr explicit strtok(Args&&... args) noexcept 
        : str_{std::forward<Args>(args)...}
    {}

    [[nodiscard]] constexpr std::string_view operator()(std::string_view const tokens) noexcept {
        return tokenize(tokens);
    }

    [[nodiscard]] constexpr std::string_view operator()(std::string_view const& str, std::string_view const tokens) noexcept {
        str_ = str;
        return tokenize(tokens);
    }

private:
    constexpr std::string_view tokenize(std::string_view const& tokens) noexcept {
        auto const begin = str_.find_first_not_of(tokens);
        auto const end = str_.find_first_of(tokens, begin);

        if (end != std::string_view::npos) {
            std::string_view const ret{str_.begin() + begin, end - begin};
            str_.remove_prefix(end);
            return ret;
        }
        else if (begin != std::string_view::npos) {
            std::string_view const ret{str_.begin() + begin, str_.size() - begin};
            str_.remove_prefix(str_.size());
            return ret;
        }
        else
            return {};
    }

    std::string_view str_;
};

std::vector<std::string_view> strtok_all(std::string_view const str, std::string_view const tokens) noexcept {
    std::vector<std::string_view> ret{};
    
    std::size_t begin{str.find_first_not_of(tokens)};
    std::size_t end{str.find_first_of(tokens, begin)};

    while (end != std::string_view::npos) { 
        ret.emplace_back(str.begin() + begin, end - begin);
        begin = str.find_first_not_of(tokens, end);
        end = str.find_first_of(tokens, begin);
    }
    if (begin != std::string_view::npos)
        ret.emplace_back(str.begin() + begin, str.size() - begin);
    return ret;
}

} // namespace extra
