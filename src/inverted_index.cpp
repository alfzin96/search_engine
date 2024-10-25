#include "inverted_index.h"
#include <mutex>
#include <thread>
#include <sstream>

bool Entry::operator==(const Entry& other) const 
{
    return (_doc_id == other._doc_id && _count == other._count);
}

bool Entry::operator<(const Entry& other) const
{
    return (_doc_id < other._doc_id);
}

Entry::Entry(int doc_id, int count) : _doc_id(doc_id), _count(count) {};


void InvertedIndex::updateDocumentBase(const std::vector<std::string>& input_docs)
{
    docs = input_docs;  
    common_freq_dictionary.clear();                                                  
    std::vector<std::thread> ths;                                         
    ths.reserve(docs.size());
    std::mutex freq_dict_access;
    for (size_t i = 0; i < docs.size(); ++i) ths.emplace_back([&, i](){
        std::stringstream wordstream(docs[i]);
        std::map<std::string, size_t> freq_dictionary;
        std::string current_word;
        while (wordstream >> current_word)
        {
            ++freq_dictionary[current_word];
        }
        for (const auto& word : freq_dictionary)
        {
            std::lock_guard<std::mutex> lock(freq_dict_access);
            common_freq_dictionary[word.first].emplace_back(i, word.second);
        }
    });
    for (auto &th : ths) th.join();
}
    
std::vector<Entry> InvertedIndex::getWordCount(const std::string& word)
{
    auto it = common_freq_dictionary.find(word);
    if (it != common_freq_dictionary.end()) return it->second;
    return {};
}

    
