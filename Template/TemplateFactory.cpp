//
// Created by michael on 15.08.15.
//

#include "TemplateFactory.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace Json;

Json::Value TemplateFactory::readTemplateFile(const char *fileName) {
    Value root;
    CharReaderBuilder readBuilder;
    readBuilder["collectComments"] = false;
    readBuilder["rejectDupKeys"] = true;
    string errorMessage;
    ifstream inStream;

    inStream.open(fileName);
    if (!Json::parseFromStream(readBuilder, inStream, &root, &errorMessage)) {
        cerr << "Error parsing template file: " << errorMessage << endl;
        throw new runtime_error(errorMessage);
    } else {
        //TODO check root value for consistency
    }
    return root;
}

vector<string> TemplateFactory::GetTemplateKeys() {
    vector<string> keys;
    keys.reserve(templateMap.size());

    for (auto kv : templateMap) {
        keys.push_back(kv.first);
    }
    return keys;
}
