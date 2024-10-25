#include <sstream>
#include <map>
#include <algorithm>
#include "search_server.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, const size_t& max_resposes)
{
    std::vector<std::vector<RelativeIndex>> result;
    result.reserve(queries_input.size());
    for (const auto& request : queries_input)
    {   
        if (request.empty())
        {
            result.push_back({});
            continue;
        }
        std::stringstream req_stream(request);          // Разбивка запроса на слова
        std::map<std::string, std::vector<Entry>> request_words;
        std::string current_word;
        while (req_stream >> current_word)
        {
            request_words.insert({current_word, _index.getWordCount(current_word)});
        }

        std::vector<std::pair<std::string, std::vector<Entry>>> sort_words;  // перенос в вектор для сортировки
        sort_words.reserve(request_words.size());
        for (const auto& word : request_words)
        {
            sort_words.push_back(word);
        }
        request_words.clear();
        
        std::sort(sort_words.begin(), sort_words.end(), // сортировка вектора в порядке увеличения частоты слов
                [](const std::pair<std::string, std::vector<Entry>>& a, const std::pair<std::string, std::vector<Entry>>& b) {
            size_t sum_count_a = 0;
            for (const auto& entry_a : a.second) sum_count_a += entry_a._count;
            size_t sum_count_b = 0;
            for (const auto& entry_b : b.second) sum_count_b += entry_b._count;
            return sum_count_a < sum_count_b;
        });
        
        std::vector<RelativeIndex> filtered_docs;     // вектор документов, в которых найдено слово
        for (const auto& entry : sort_words[0].second) filtered_docs.push_back({entry._doc_id, 0});
        
        for (size_t i = 1; i < sort_words.size() && !filtered_docs.empty(); ++i)
        {   
            for (size_t j = 0; j < filtered_docs.size();)
            {
                bool find = false;
                for (const auto& entry : sort_words[i].second)
                {
                    if (filtered_docs[j].doc_id == entry._doc_id)
                    {
                        find = true;
                        break;
                    }
                }
                if (!find)
                {
                    std::swap(filtered_docs[j], filtered_docs.back());
                    filtered_docs.pop_back();
                }
                else ++j;
            } 
        }

        if (filtered_docs.empty())     //  если по запросу документов нет, релевантность не считаем, вставляем пустой вектор
        {
            result.push_back(filtered_docs);
            continue;
        }

        size_t max_relevance = 0;
        for (auto& doc : filtered_docs)         // считаем абсолютную релевантность
        {
            for (const auto& word : sort_words)
            {
                for (const auto& entry : word.second)
                {
                    if (entry._doc_id == doc.doc_id)
                    {
                        doc.rank += entry._count;
                        break;
                    }
                }
            }
            if (doc.rank > max_relevance) max_relevance = doc.rank;
        }

        for (auto& doc : filtered_docs) doc.rank /= max_relevance; // считаем относительную релевантность

        std::sort(filtered_docs.begin(), filtered_docs.end(),       // сортировка по релевантности
                [](const RelativeIndex& a, const RelativeIndex& b){
            return a.rank > b.rank;
        });

        if (filtered_docs.size() > max_resposes) filtered_docs.resize(max_resposes);
        result.push_back(filtered_docs);
    }
    return result;
}