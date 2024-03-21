#include <gtest/gtest.h>
#include <fstream>
#include "../inc/json.hh"

using namespace beiklive::json;

TEST(json_func, parse)
{
    std::ifstream f("/home/byeaxj/WorkSpace/MyCode/MyToolModule-/res/json_example.json");
    json myjson = parse(f);

    EXPECT_EQ(myjson.getParseResult(), PARSE_TYPE::JSON_PARSE_OK);
}

std::string getSubString(const std::string& str, int start, int size)
{
    return str.substr(start, size + 1);
}

TEST(json_func, typePrase)
{
    json myjson;
    int gap = 0;
    int index = 0;
    std::string substr;
    std::string str = "null\"string\"true[[[1]]]0.1234{\"name\": \"john\"}";
    EXPECT_EQ(myjson.parse_null(str, 0, &gap), PARSE_TYPE::JSON_PARSE_OK);
    EXPECT_EQ(getSubString(str, 0, gap), "null");

    EXPECT_EQ(myjson.parse_string(str, 4, &gap), PARSE_TYPE::JSON_PARSE_OK);
    EXPECT_EQ(getSubString(str, 4, gap), "\"string\"");

    EXPECT_EQ(myjson.parse_boolean(str, 12, &gap), PARSE_TYPE::JSON_PARSE_OK);
    EXPECT_EQ(getSubString(str, 12, gap), "true");

    EXPECT_EQ(myjson.parse_array(str, 16, &gap), PARSE_TYPE::JSON_PARSE_OK);
    EXPECT_EQ(getSubString(str, 16, gap), "[[[1]]]");

    EXPECT_EQ(myjson.parse_number(str, 23, &gap), PARSE_TYPE::JSON_PARSE_OK);
    EXPECT_EQ(getSubString(str, 23, gap), "0.1234");

    EXPECT_EQ(myjson.parse_json(str, 29, &gap), PARSE_TYPE::JSON_PARSE_OK);
    EXPECT_EQ(getSubString(str, 29, gap), "{\"name\": \"john\"}");
}





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();   
}