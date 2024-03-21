#include "../inc/Translator.hh"
#include "../inc/Common.h"

EXAMPLE_MAIN_START(translator_main)

    // 创建 Translator 对象，并指定翻译文件的路径
    beiklive::Translator TR("/home/byeaxj/WorkSpace/MyCode/MyToolModule-/res/translate/");

    auto language_list = {beiklive::LANGUAGE_TYPE::EN, beiklive::LANGUAGE_TYPE::CH, beiklive::LANGUAGE_TYPE::JP};


    for(auto& item : language_list)
    {
        TR.Tr_setLanguageType(item);
        std::cout << "Current Language: " << TR.TR("Language") << std::endl;
        std::cout << "Welcome Message: " << TR.TR("WelcomeMessage") << std::endl;
    }

EXAMPLE_MAIN_FINISH(translator_main)