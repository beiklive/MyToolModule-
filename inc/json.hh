// Copyright (c) RealCoolEngineer. 2024. All rights reserved.
// Author: beiklive
// Date: 2024-02-02
#ifndef INC_JSON_HH_
#define INC_JSON_HH_
#include <iostream>
#include <fstream>
#include <sstream>

namespace beiklive
{
    namespace json
    {
        class json;

        // enum types
        enum class PARSE_TYPE
        {
            JSON_PARSE_OK,
            JSON_PARSE_EXPECT_VALUE,
            JSON_PARSE_INVALID_VALUE,
            JSON_PARSE_WRONG_FORMAT
        };

        enum class JSONOBJ_TYPE
        {
            JSON_OBJ_TYPE_NULL,
            JSON_OBJ_TYPE_NUMBER,
            JSON_OBJ_TYPE_STRING,
            JSON_OBJ_TYPE_BOOL,
            JSON_OBJ_TYPE_ARRAY,
            JSON_OBJ_TYPE_OBJECT
        };


        namespace
        {
            const size_t STRING_MAX_SIZE{ 65535 };
        }

        class json
        {
        private:
            PARSE_TYPE _parseResult;
            std::string _fileContent;

        public:
            PARSE_TYPE parse_null(const std::string& str, const int& index, int* gapIndex)
            {

                        *gapIndex = 0;
                if (str[index] == 'n' && str[index + 1] == 'u' && str[index + 2] == 'l' && str[index + 3] == 'l')
                {
                    *gapIndex += 3;
                    return PARSE_TYPE::JSON_PARSE_OK;
                }
                else
                {
                    *gapIndex = 0;
                    return PARSE_TYPE::JSON_PARSE_INVALID_VALUE;
                }
            }

            PARSE_TYPE parse_string(const std::string& str, const int& index, int* gapIndex)
            {
                int count = 0;
                        *gapIndex = 0;
                do {
                    count++;
                } while (str[index + count] != '"' && count <= STRING_MAX_SIZE && count + index < str.size());
                if (count > STRING_MAX_SIZE || count + index > str.size())
                {
                    *gapIndex = 0;
                    return PARSE_TYPE::JSON_PARSE_INVALID_VALUE;
                }
                else
                {
                    *gapIndex = count;
                    return PARSE_TYPE::JSON_PARSE_OK;
                }
            }

            PARSE_TYPE parse_boolean(const std::string& str, const int& index, int* gapIndex)
            {

                        *gapIndex = 0;
                if (str[index] == 't' && str[index + 1] == 'r' && str[index + 2] == 'u' && str[index + 3] == 'e')
                {
                    *gapIndex += 3;
                    return PARSE_TYPE::JSON_PARSE_OK;
                }
                else if (str[index] == 'f' && str[index + 1] == 'a' && str[index + 2] == 'l' && str[index + 3] == 's' && str[index + 4] == 'e')
                {
                    *gapIndex += 4;
                    return PARSE_TYPE::JSON_PARSE_OK;
                }
                else
                {
                    *gapIndex = 0;
                    return PARSE_TYPE::JSON_PARSE_INVALID_VALUE;
                }
            }

            PARSE_TYPE parse_array(const std::string& str, const int& index, int* gapIndex)
            {
                int left_open_bracket = 0;
                int count = 0;
                        *gapIndex = 0;
                while (1)
                {
                    if (count > STRING_MAX_SIZE || count + index > str.size())
                    {
                        *gapIndex = 0;
                        return PARSE_TYPE::JSON_PARSE_INVALID_VALUE;
                    }

                    
                    switch (str[index + count])
                    {
                    case '[':
                        left_open_bracket++;
                        break;
                    case ']':
                        left_open_bracket--;
                        break;
                    default:
                        break;
                    }

                    if(left_open_bracket == 0)
                    {
                        *gapIndex = count;
                        return PARSE_TYPE::JSON_PARSE_OK;
                    }
                    count++;
                }
            }

            bool isCharNumber(const char& c)
            {
                return (c >= '0' && c <= '9');
            }

            PARSE_TYPE parse_number(const std::string& str, const int& index, int* gapIndex)
            {
                int numberCount = 0;
                int count = 0;

                        *gapIndex = 0;
                // 0123  is invalid
                if(str[index] == '0' && isCharNumber(str[index + 1]))
                {
                    return PARSE_TYPE::JSON_PARSE_INVALID_VALUE; 
                }

                while (1)
                {
                    if (count > STRING_MAX_SIZE || count + index > str.size())
                    {
                        *gapIndex = 0;
                        std::cout << "count:" << count << " size: " << str.size()<< std::endl;
                        return PARSE_TYPE::JSON_PARSE_INVALID_VALUE;
                    }
                    switch (isCharNumber(str[index + count]))
                    {
                    case false:
                        if (str[index + count] == '.')
                        {
                            break;
                        }
                        else
                        {
                            *gapIndex = count - 1;
                            return PARSE_TYPE::JSON_PARSE_OK;
                        }
                        break;
                    default:
                        break;
                    }
                    count++;
                }
            }

            PARSE_TYPE parse_json(const std::string& str, const int& index, int* gapIndex)
            {
                int left_open_bracket = 0;
                int count = 0;
                while (1)
                {
                    if (count > STRING_MAX_SIZE || count + index > str.size())
                    {
                        *gapIndex = 0;
                        return PARSE_TYPE::JSON_PARSE_INVALID_VALUE;
                    }

                    
                    switch (str[index + count])
                    {
                    case '{':
                        left_open_bracket++;
                        break;
                    case '}':
                        left_open_bracket--;
                        break;
                    default:
                        break;
                    }

                    if(left_open_bracket == 0)
                    {
                        *gapIndex = count;
                        return PARSE_TYPE::JSON_PARSE_OK;
                    }
                    count++;
                }
            }

            



            void parseBuffer(const std::string& str)
            {
                PARSE_TYPE ret = PARSE_TYPE::JSON_PARSE_OK;
                int gapIndex = 0;
                for (size_t i = 0; i < str.size() && (ret == PARSE_TYPE::JSON_PARSE_OK); ++i) {
                    // std::cout << "Character at index " << i << ": " << str[i] << std::endl;
                    gapIndex = 0;
                    switch (str[i])
                    {
                        // PARSE null
                    case 'n':
                        ret = parse_null(str, i, &gapIndex);
                        /* code */
                        break;
                        // PARSE string
                    case '"':
                        ret = parse_string(str, i, &gapIndex);
                        break;
                        // PARSE bool
                    case 't':
                    case 'f':
                        ret = parse_boolean(str, i, &gapIndex);
                        break;
                        // PARSE array
                    case '[':
                        ret = parse_array(str, i, &gapIndex);
                        break;
                        // PARSE number
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        ret = parse_number(str, i, &gapIndex);
                        break;
                        // PARSE json
                    case '{':
                        ret = parse_json(str, i, &gapIndex);
                        break;
                        // PARSE space and symbol
                    case ' ':
                    case '\t':
                    case '\n':
                    case '\r':
                        gapIndex = 1;
                        break;
                    case ':':

                        break;
                    case ',':

                        break;
                    default:
                        break;
                    }
                    i += gapIndex;
                }
            }



        public:
            json() = default;
            ~json() = default;


            PARSE_TYPE parse(const std::ifstream& f)
            {
                if (f.is_open())
                {
                    std::stringstream buffer;  // 创建一个字符串流用于存储文件内容
                    buffer << f.rdbuf();  // 将文件内容读取到字符串流中
                    _fileContent = buffer.str();

                }
                _parseResult = PARSE_TYPE::JSON_PARSE_OK;
                return _parseResult;
            }

            PARSE_TYPE getParseResult()
            {
                return _parseResult;
            }


        };





        // public function
        json parse(const std::ifstream& f)
        {
            json _jsonObject;
            PARSE_TYPE ret = _jsonObject.parse(f);
            return _jsonObject;
        }






    } // namespace json
} // namespace beiklive


#endif  // INC_JSON_HH_
