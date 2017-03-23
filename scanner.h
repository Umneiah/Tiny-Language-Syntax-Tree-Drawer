#ifndef SCANNER_H
#define SCANNER_H

class Scanner
{
public:
    string parserinput;


    void DealWithCode(string S)
    {
        bool reserved = 0;
        string temp, empty;
        empty = "";
        int i = 0;
        while (state != DONE) {
            switch (state) {
            case START:
                if (IsSpace(S[i])) {
                    i++;
                    if (i == S.length()) state = DONE;
                    else state = START;
                }
                else if (IsDigit(S[i])) {
                    state = INNUM;
                }
                else if (IsLetter(S[i])) {
                    state = INID;
                }
                else if (S[i] == ':') {
                    state = INASSIGN;
                }
                else if (S[i] == '{') {
                    i++;
                    state = INCOMMENT;
                }
                //other
                else if (IsSymbol(S[i])) {

                    switch (S[i]) {
                        //if semi colon we should start new line but if any thing else just put space
                    case ';': parserinput = parserinput + " " + S[i] + " "; break;
                    default: parserinput = parserinput + " " + S[i] + " "; break;
                    }
                    i++;
                    if (i == S.length()) state = DONE;
                    else state = START;
                }
                else state = DONE;
                break;

            case INCOMMENT:
                if (state == INCOMMENT) {
                    while (S[i] != '}') {
                        i++;
                    }
                    i++; //43an an2el lly ba3d el comment kolo
                    if (i == S.length()) state = DONE;
                    else state = START;
                }
                break;

            case INNUM:
                while (IsDigit(S[i])) {
                    temp += S[i];
                    i++;
                }
                parserinput = parserinput + "number{" + temp + "} ";
                temp = ""; //ha empty eltemp dah 34an lma agy ast3mloh tany b2a
                if (i == S.length()) state = DONE;
                else state = START;
                break;

            case INID:
                while (IsLetter(S[i])) {
                    temp += S[i];
                    i++;
                }
                for (int i = 0; i < 8; i++)
                {
                    if (ReservedWords[i] == temp) reserved = 1;
                }
                if (reserved) //reserved
                {
                    //put newline before reserved
                    parserinput = parserinput + temp + " ";
                }
                else  //identifier
                {
                    parserinput = parserinput + "identifier{" + temp + "} ";
                }
                temp = "";
                reserved = 0;
                if (i == S.length()) state = DONE;
                else state = START;
                break;

            case INASSIGN:
                if (S[i] == ':') {
                    i += 2;
                    parserinput = parserinput + ":= ";
                    state = START;
                }
                else {
                    if (i == S.length()) state = DONE;
                    else state = START;
                }
                break;

            case DONE:
                break;
            }
        }
    }


private:
    string ReservedWords[8] = { "if","then","else","end","repeat","until","read","write" };
    enum DFAstates { START, INCOMMENT, INNUM, INID, INASSIGN, DONE };
    DFAstates state = START; //set START to be intial state

    bool IsSymbol(char c)
    {
        return (c == '+' || '-' || '*' || '/' || '=' || '<' || '>' || '(' || ')' || ';');
    }

    bool IsDigit(char d)
    {
        return (d >= '0' && d <= '9');
    }

    bool IsLetter(char l)
    {
        return (l >= 'a' && l <= 'z' || l >= 'A' && l <= 'Z');
    }

    bool IsSpace(char s)
    {
        //add \t and \n because of QT
        return (s == ' ' || s == '\t' || s == '\n');
    }

};

#endif // SCANNER_H
