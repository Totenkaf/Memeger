#pragma once

#include <iostream>
#include <vector>
#include <string>

class Search {
private:
    std::string query;
public:
    Search() = default;

    ~Search() = default;

    Search(const Search &search);

    vector <string> search_by_query(std::string query);

    std::string get_query();

    void set_query(std::string new_query);
};
