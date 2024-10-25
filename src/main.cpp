#include <iostream>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "converter_json.h"
#include "inverted_index.h"
#include "search_server.h"


int main()
{
    ConverterJSON con;
    InvertedIndex idx;
    idx.updateDocumentBase(con.getTextDocuments());
    std::cout << "Indexing completed\n";
    SearchServer srv(idx);
    auto result = srv.search(con.getRequests(), con.getResponsesLimit());
    std::cout << "Searching completed\n";
    con.putAnswers(result);
    std::cout << "Result is saved in file \"answers.json\"\n";
}
