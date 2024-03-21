// Copyright (c) RealCoolEngineer. 2024. All rights reserved.
// Author: beiklive
// Date: 2024-02-21
#ifndef INC_TRANSLATER_HPP_
#define INC_TRANSLATER_HPP_

#include <fstream>
#include <iostream>
#include <vector>
#include "./nlohmann/json.hpp"

namespace beiklive {

    enum LANGUAGE_TYPE {
        EN,
        CH,
        JP
    };

    std::vector<std::string> LANGLUAGE_LIST{
    "EN",
    "CH",
    "JP"
    };

    class JsonReader {
        using json = nlohmann::json;
    private:
        json data;
    public:
        JsonReader() = default;
        void SetJsonFile(const std::string& file) {
            data.clear();
            std::ifstream f(file + ".json");
            if (!f.is_open()) {
                throw std::runtime_error("Failed to open JSON file: " + file);
            }
            data = json::parse(f);
        }

        std::string GetJsonString(const std::string& key) const {
            auto it = data.find(key);
            if (it != data.end()) {
                return it->get<std::string>();
            }
            else {
                throw std::runtime_error("Key not found in JSON data: " + key);
            }
        }
    };

    class Translator
    {
    private:
        std::string _transfilePath;
        std::string _typeName{ "EN" };
        LANGUAGE_TYPE _curLanguage{ EN };
        JsonReader _jsonReaderObj;
    public:
        Translator(const std::string& filePath) {
            _transfilePath = filePath;
        }
        ~Translator() = default;


        inline void Tr_setLanguageType(LANGUAGE_TYPE type)
        {
            _curLanguage = type;
            _typeName = LANGLUAGE_LIST[_curLanguage];
            _jsonReaderObj.SetJsonFile(_transfilePath + _typeName);
        }

        std::string TR(const std::string& sentence)
        {
            return _jsonReaderObj.GetJsonString(sentence);
        }

    };
};
#endif  // INC_TRANSLATER_HPP_
