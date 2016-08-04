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
        pattern = "!<c(o|u)(b|k|kk|ck|gr|x|z|zz|s|ss)<(a|u|o)C|('|-)!C>v<|x|z|b|r|k>>";
    } else if (nameType == NameType::DARK_THING) {
        pattern = "!(O|U|((Kr|X|B|G)<v>))csv";
    } else if (nameType == NameType::ALIEN) {
        pattern = "!<BV-!<s|sv>|BV'!<s|sv|d>>";
    } else if (nameType == NameType::CORPORATION) {
        pattern = "!<v|<C|c>a>s(<u|a|c>|i|us) (Corp|Inc|Tech|Ltd|LLC).";
    } else if (nameType == NameType::FUNNY) {
        pattern = "!<m|s|i><M|is>|!md !Md";
    } else if (nameType == NameType::LIGHT_PERSON) {
        pattern = "!(a|i|y|e)(y|l|m|s)('<!s>|e|i|l)s(|-<!(a|i|e)(i|y)s>)";
    } else if (nameType == NameType::LIGHT_THING) {
        pattern = "!(a|e|i|y|a|e)(y|l|m)(e|i|l|a)<s|v|V>(i|y|u|a)(m|s|)";
    } else if (nameType == NameType::SHIP) {
        pattern = "!(m|v|p|x|l|c)(<v>r|<v>l<v>|a|y|e)(m|n) !sv(p|k|t)";
    } else if (nameType == NameType::CUDDLY) {
        pattern = "!(m|v|p|a|l|u)(r|li|a|y|)m";
    } else if (nameType == NameType::CUDDLY_VERB) {
        pattern = "(m|v|p|a|l|u)(r|li|a|y|)m";
    } else if (nameType == NameType::VENERABLE) {
        pattern = "!((|(<B>|s|h|ty|ph|r))(i|ae|ya|ae|eu|ia|i|eo|ai|a)(lo|la|sri|da|dai|the|sty|lae|due|li|lly|ri|na|ral|sur|rith)(|(su|nu|sti|llo|ria|))(|(n|ra|p|m|lis|cal|deu|dil|suir|phos|ru|dru|rin|raap|rgue))) !<s<v|V>(tia)|s<v|V>(os)|B<v|V>c(ios)|B<v|V><c|C>v(ios|os)>";
    }

    TokenNameGenerator nameGenerator(pattern);
    return nameGenerator.toString(random);
}

