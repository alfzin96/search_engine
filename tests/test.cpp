#include <set>
#include "search_server.h"
#include "inverted_index.h"
#include "gtest/gtest.h"


void TestInvertedIndexFunctionality(const std::vector<std::string>& docs, const std::vector<std::string>& requests, const std::vector<std::set<Entry>>& expected)
{
    std::vector<std::set<Entry>> result;
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    for(const auto& request : requests) {
        std::vector<Entry> word_count = idx.getWordCount(request);
        std::set<Entry> word_count_set;
        for (const auto& entry : word_count) word_count_set.insert(entry);
        result.push_back(word_count_set);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {"london is the capital of great britain", "big ben is the nickname for the Great bell of the striking clock"};
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::set<Entry>> expected = {
        {
            {0, 1}
        }, {
            {0, 1}, {1, 3}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappucсino"
    };
    const std::vector<std::string> requests = {"milk", "water", "cappucсino"};
    const std::vector<std::set<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        }, {
            {0, 3}, {1, 2}, {2, 5}
        }, {
            {3, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::set<Entry>> expected = {
        {}, {
            {1, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar", ""};
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {2, 1}, {0, 0.7}, {1, 0.3}
        }, {
        }, {
        }
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request, 5);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain is is is is is is",
        "paris is the capital of france",
        "berlin is the capital of germany is is is is",
        "rome is the capital of italy",
        "madrid is the capital of spain is is is",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland is",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine is is",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };
    const std::vector<std::string> request = {"is the capital of"};
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {0, 1},
            {2, 0.8},
            {4, 0.7},
            {8, 0.6},
            {6, 0.5}
        }
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request, 5);
    ASSERT_EQ(result, expected);
}

