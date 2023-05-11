#include "table.h"

Table::Table() { this->obDb = new ObDb(); }
Table::~Table() { delete this->obDb; }
string Table::dealStr(const string &str) {
    string resStr;

    for (int i = 0; i < str.length(); i++) {
        switch ((int)str[i]) {
        case 0:
            resStr += "\\0";
            break;
        case 8:
            resStr += "\\b";
            break;
        case 9:
            resStr += "\\t";
            break;
        case 10:
            resStr += "\\n";
            break;
        case 13:
            resStr += "\\r";
        case 26:
            resStr += "\\Z";
            break;
        case 34:
            resStr += "\\\"";
            break;
        case 37:
            resStr += "\\%";
        case 39:
            resStr += "\\\'";
            break;
        case 45:
            resStr += "\\-";
            break;
        case 92:
            resStr += "\\\\";
            break;
        case 95:
            resStr += "\\_";
            break;
        default:
            resStr += str[i];
            break;
        }
    }
    resStr = "\"" + resStr + "\"";
    return resStr;
}
