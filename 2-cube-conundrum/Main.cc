#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

struct GameInfo
{
    int red{};
    int green{};
    int blue{};
};

std::optional<int> ParseInt(std::string_view str)
{
    if (str.empty())
        return {};

    char *end_ptr = nullptr;

    std::string s{str};
    auto const conv = std::strtol(s.c_str(), &end_ptr, 10);

    if (*end_ptr != 0)
        return {};

    return {conv};
}

void Split(std::vector<std::string_view> &dest, std::string_view str, std::string_view delimiter)
{
    size_t const delimiter_size = delimiter.size();
    std::string_view word;
    while (true)
    {
        std::size_t end = str.find(delimiter);
        word = str.substr(0, end);
        str = str.substr(end + delimiter_size);
        dest.push_back(word);

        if (end == std::string::npos)
            break;
    }
}

std::string_view RTrim(std::string_view str, char const ch)
{
    std::size_t i = 0;
    while (!str.empty() && str.at(i) == ch)
        i++;

    return str.substr(i);
}

std::pair<std::string_view, std::string_view> SplitLeftOnce(std::string_view str, std::string_view delimiter)
{
    std::size_t const i = str.find(delimiter);
    if (i == std::string::npos)
    {
        return {str, ""};
    }

    return {str.substr(0, i), str.substr(i + delimiter.size())};
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cout << "args\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (input.bad())
    {
        std::cout << "bad file\n";
        return 1;
    }

    std::string line;
    int sum_of_ids = 0;
    std::vector<std::string_view> game_infos;
    std::vector<std::string_view> game_split;

    while (std::getline(input, line))
    {
        std::cout << "Line: " << line << '\n';

        auto const &[left, right] = SplitLeftOnce(line, ": ");

        if (right.empty())
        {
            throw std::runtime_error("no game info");
        }

        auto const& [_, game_id_str] = SplitLeftOnce(left, " ");
        auto const id_int = ParseInt(game_id_str);
        if (!id_int.has_value()) throw std::runtime_error("this game has no ID");

        game_infos.clear();
        bool game_ok = true;
        Split(game_infos, right, std::string_view(";"));

        for (auto const game_info : game_infos)
        {
            auto const game_info_trimmed = RTrim(game_info, ' ');
            game_split.clear();
            Split(game_split, game_info_trimmed, ",");

            GameInfo game{};
            for (auto const game_split_part : game_split)
            {
                auto const game_split_part_trimmed = RTrim(game_split_part, ' ');
                std::cout << std::quoted(game_split_part_trimmed) << '\n';
                auto const &[number, color] = SplitLeftOnce(game_split_part_trimmed, " ");
                auto number_int = ParseInt(number);

                if (!number_int.has_value())
                {
                    throw std::runtime_error("can't convert number");
                }

                std::cout << "Number: " << number_int.value() << ", color: " << color << '\n';

                if (color == "red")
                {
                    game.red = number_int.value();
                }
                else if (color == "green")
                {
                    game.green = number_int.value();
                }
                else if (color == "blue")
                {
                    game.blue = number_int.value();
                } else {
                    throw std::runtime_error("what is this color");
                }
            }

            if (game.red > 12 || game.green > 13 || game.blue > 14)
            {
                game_ok = false;
                break;
            }
        }

        if (game_ok)
        {
            sum_of_ids += id_int.value();
        }
    }

    std::cout << "idsum = " << sum_of_ids << "\n";

    return 0;
}