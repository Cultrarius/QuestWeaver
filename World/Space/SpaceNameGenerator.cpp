//
// Created by michael on 15.03.16.
//

#include "World/Space/SpaceNameGenerator.h"
#include <algorithm>
#include <Core/NameGen/TokenNameGenerator.h>

using namespace weave;
using namespace std;

std::string weave::SpaceNameGenerator::CreateName(NameType nameType, std::shared_ptr<RandomStream> random) const {

    string pattern = "<V|v><ss|s>(a|us|<V>)";
    if (nameType == NameType::DARK_PERSON) {
        pattern = "<c(o|u)(b|k|kk|ck|gr|x|z|zz|s|ss)<(a|u|o)C|('|-)!C>v<|x|z|b|r|k>>";
    } else if (nameType == NameType::DARK_THING) {
        pattern = "(O|U|((Kr|X|B|G)<v>))csv";
    } else if (nameType == NameType::ALIEN) {
        pattern = "<BV-!<s|sv>|BV'!<s|sv|d>>";
    } else if (nameType == NameType::CORPORATION) {
        pattern = "<v|<C|c>a>s(<u|a|c>|i|us) (Corp|Inc|Tech).";
    } else if (nameType == NameType::FUNNY) {
        pattern = "<m|s|i><M|is>|md !Md";
    } else if (nameType == NameType::LIGHT_PERSON) {
        pattern = "(a|i|y|e)(y|l|m|s)('<!s>|e|i|l)s(|-<!(a|i|e)(i|y)s>)";
    } else if (nameType == NameType::LIGHT_THING) {
        pattern = "(a|e|i|y|a|e)(y|l|m)(e|i|l|a)<s|v|V>(i|y|u|a)(m|s|)";
    }

    TokenNameGenerator nameGenerator(pattern);
    string name = nameGenerator.toString(random);
    name[0] = static_cast<char>(toupper(name[0]));
    return name;
}

