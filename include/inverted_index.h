#pragma once
#include <vector>
#include <string>
#include <map>

struct Entry {
    size_t _doc_id, _count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator==(const Entry& other) const;
    bool operator<(const Entry& other) const;
    Entry(int doc_id, int count);
};
class InvertedIndex {
public:
    InvertedIndex() = default;
    /**
    * Обновить или заполнить базу документов, по которой будем совершать
    поиск
    * @param texts_input содержимое документов
    */
    void updateDocumentBase(const std::vector<std::string>& input_docs);
    /**
    * Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> getWordCount(const std::string& word);
private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> common_freq_dictionary; // частотный словарь
};
