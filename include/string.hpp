// string.hpp

#pragma once

#include <string_view>
#include <vector>

namespace extra {

class strtok {
public:
    template<class Str, class Tok>
    constexpr explicit strtok(Str&& str, Tok&& tokens) noexcept 
        : str_{std::forward<Str>(str)}
        , tokens_{std::forward<Tok>(tokens)}
    {}

    [[nodiscard]] constexpr std::string_view operator()() noexcept {
        auto const begin = str_.find_first_not_of(tokens_);
        auto const end = str_.find_first_of(tokens_, begin);

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

private:
    std::string_view str_;
    std::string_view const tokens_;
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