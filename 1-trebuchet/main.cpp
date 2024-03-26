#include <fstream>
#include <iostream>
#include <unordered_map>

const std::unordered_map<std::string, int> dignames =
        {
                {"one", 1},
                {"two", 2},
                {"three", 3},
                {"four", 4},
                {"five", 5},
                {"six", 6},
                {"seven", 7},
                {"eight", 8},
                {"nine", 9}};

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cout << "args!\n";
        std::exit(1);
    }

    std::ifstream input_file(argv[1]);
    if (input_file.bad()) {
        std::cout << "bad input\n";
        std::exit(1);
    }

    std::string line;
    int line_number = 0;
    int sum = 0;

    while (std::getline(input_file, line)) {
        line_number++;
        int encountered_digits = 0;
        int last_digit = 0;
        int first = 0;
        size_t cursor = 0;

        while (cursor < line.size()) {
            bool found_digit = false;
            char curr = line.at(cursor);
            if (isdigit(curr)) {
                encountered_digits++;
                last_digit = curr - '0';
            } else {
                for (auto const &[digname, dig]: dignames) {
                    if (line.substr(cursor).starts_with(digname)) {
                        encountered_digits++;
                        last_digit = dig;
                        break;
                    }
                }
            }

            if (encountered_digits == 1) {
                first = last_digit;
            }

            cursor++;
        }

        sum += first * 10 + last_digit;
    }

    std::cout << "Lines: " << line_number << '\n';
    std::cout << "Sum:   " << sum << '\n';
    return 0;
}
