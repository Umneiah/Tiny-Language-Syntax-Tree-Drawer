#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <list>

using namespace std;


class Node
{

public:
    enum NodeType
    {
        Undefined, Box, Circle
    };

    NodeType Type;
    string   Title, Details;
    Node*    Left;
    Node*    Right;
    Node*    ElsePart;
    list<Node*>    NextTo;

    Node()
    {
        Type = Undefined;
        Title = " ";
        Details = " ";
        Left = NULL;
        Right = NULL;
        ElsePart = NULL;
    }

    ~Node()
    {
        delete Left;
        delete Right;
        delete ElsePart;
    }
};

class Parser
{
public:
    string token;
    Parser()
    {
        z = 0;
    }

    // pair of the token <type , value of it>
    pair <string, string> mypair;
    vector<pair<string, string>> v;

    void cutting_parsing_input(string m)
    {
        istringstream iss(m);
        while (iss)
        {
            iss >> token;
            tokensvec.push_back(token);
        }
        token = tokensvec[z];
        mypair = make_pair(token, token);
        v.push_back(mypair);
    }
    // program -> stmt-seq
    Node* prog()
    {
        Node* Program = new Node();
        Program = stmt_seq();
        cout << endl << "Program found" << endl;
        return Program;
    }


private:
    int z;
    enum stmtTypes { IF, REPEAT, ASSIGN, READ, WRITE, ERROR };
    vector <string> tokensvec;
    string tokentemp;
    string value;


    void match(string s)
    {
        if (s == v[z].first)
        {
            z++;

            tokentemp = tokensvec[z];
            if (tokentemp.find_first_of("{") != NULL)
            {
                size_t posstrt = tokentemp.find("{") + 1;
                size_t posend = tokentemp.find("}");
                size_t lenvalue = posend - posstrt;
                value = tokentemp.substr(posstrt, lenvalue  );
                value = "{" + value + "}";
                token = tokentemp.substr(0 , posstrt - 1);

            }
            else
            {
                token = tokentemp;
            }

            mypair = make_pair(token , value );
            v.push_back(mypair);
        }
        else
        {
            cout << endl << "not found" << endl;
            exit(1);
        }
    }

    stmtTypes getStmtType(string s)
    {
        if (s == "if") return IF;
        if (s == "identifier") return ASSIGN;
        if (s == "read") return READ;
        if (s == "write") return WRITE;
        if (s == "repeat") return REPEAT;
        else return ERROR;
    }

    // stmt-seq -> stmt {; stmt}
    //done
    Node* stmt_seq()
    {
        Node* StmtSeq = new Node();
        StmtSeq = stmt();
        while (token == ";")
        {
            match(";");
            StmtSeq->NextTo.push_back(stmt());
        }
        return StmtSeq;
    }

    // stmt -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
    //done
    Node* stmt()
    {
        Node* Stmt = new Node();
        switch (getStmtType(token))
        {
        case IF:
            Stmt = if_stmt(); break;
        case REPEAT:
            Stmt = repeat_stmt(); break;
        case ASSIGN:
            Stmt = assign_stmt(); break;
        case READ:
            Stmt = read_stmt(); break;
        case WRITE:
            Stmt = write_stmt(); break;
        case ERROR: // el mafrod el repeat leha 2 children
            cout << "no stmt found"; exit(1); break;
        default:
            break;
        }
        return Stmt;
    }

    // if-stmt -> if exp then stmt-seq [else stmt-seq] end
    //done
    Node* if_stmt()
    {
        Node* IfStmt = new Node();
        IfStmt->Type = IfStmt->Box;
        IfStmt->Title = "if";
        match("if");
        IfStmt->Left = exp();
        match("then");
        IfStmt->Right = stmt_seq();
        if (token == "else")
        {
            match("else");
            //Node* Else = new Node();
            //Else->Left = stmt_seq();
            //Else->Title = "else";
            IfStmt->ElsePart = stmt_seq();
        }
        match("end");
        cout << "if statement end" << endl;
        return IfStmt;
    }

    // repeat-stmt -> repeat stmt-seq until exp
    //done
    Node* repeat_stmt()
    {
        Node* RepeatStmt = new Node();
        RepeatStmt->Type = RepeatStmt->Box;
        RepeatStmt->Title = "repeat";
        match("repeat");
        RepeatStmt->Left = stmt_seq();
        match("until");
        RepeatStmt->Right = exp();
        cout << "repeat end" << endl;
        return RepeatStmt;
    }

    // assign-stmt -> identifier := exp
    //done
    Node* assign_stmt()
    {
        Node* AssignStmt = new Node();
        AssignStmt->Type = AssignStmt->Box;
        AssignStmt->Title = "assign";
        AssignStmt->Details = v[z].second;
        match("identifier");
        match(":=");
        AssignStmt->Left = exp();

        cout << "assignment end" << endl;
        return AssignStmt;
    }

    // read-stmt -> read identifier
    //done
    Node* read_stmt()
    {
        Node* ReadStmt = new Node();
        ReadStmt->Type = ReadStmt->Box;
        match("read");
        ReadStmt->Title = "read";
        ReadStmt->Details = v[z].second;
        match("identifier");
        cout << "read end" << endl;
        return ReadStmt;
    }

    // write-stmt -> write exp
    //done
    Node* write_stmt()
    {
        Node* WriteStmt = new Node();
        WriteStmt->Type = WriteStmt->Box;
        match("write");
        WriteStmt->Left = exp();
        WriteStmt->Title = "write";
        cout << "write end" << endl;
        return WriteStmt;
    }

    // exp -> simple-exp [comparison-op simple-exp]
    //done
    Node* exp()
    {
        Node* Exp = new Node();
        Exp = simple_exp();
        if (token == "<" || token == "=")
        {
            Node* Expo = new Node();
            Expo = Exp;
            Exp = comparison_op();//el write de el mafrod yeb2a leha left ell hwa rag3 mn exp
            // w hwa m4 mawgood w f 7eta tani bardo feha nafs el mo4kla
            Exp->Left = Expo;
            Exp->Right = simple_exp();
        }
        cout << "exp end" << endl;
        return Exp;
    }

    // comparison-op -> < | =
    //done
    Node* comparison_op()
    {

        Node* ComparisonOp = new Node();
        ComparisonOp->Type = ComparisonOp->Circle;
        ComparisonOp->Title = "op";
        if (token == "<")
        {
            match("<");
            ComparisonOp->Details = "{<}";
        }
        if (token == "=")
        {
            match("=");
            ComparisonOp->Details = "{=}";
        }
        return ComparisonOp;
    }

    // simple-exp -> term { addop term }
    //done
    Node* simple_exp()
    {
        Node* SimpleExp = new Node();
        SimpleExp->Type = SimpleExp->Circle;
        SimpleExp = term();
        while (token == "+" || token == "-")
        {
            Node* SimpleExpo = new Node();
            SimpleExpo = SimpleExp;
            SimpleExp = addop();
            SimpleExp->Left = SimpleExpo;
            SimpleExp->Right = term();
        }
        cout << "simple exp end" << endl;
        return SimpleExp;
    }

    // addop -> + | -
    //done
    Node* addop()
    {
        Node* AddOp = new Node();
        AddOp->Type = AddOp->Circle;
        AddOp->Title = "op";
        if (token == "+")
        {
            match("+");
            AddOp->Details = "{+}";
        }
        if (token == "-")
        {
            match("-");
            AddOp->Details = "{-}";
        }
        return AddOp;
    }

    // term -> factor { mulop factor }
    //done
    Node* term()
    {
        Node* Term = new Node();
        Term->Type = Term->Circle;
        Term =factor();
        while (token == "*" || token == "/")
        {
            Node* Termo = new Node();
            Termo = Term;
            Term = mulop();
            Term->Left = Termo;
            Term->Right = factor();
        }
        cout << "term end" << endl;
        return Term;
    }

    // mulop -> * | /
    //done
    Node* mulop()
    {
        Node* MulOp = new Node();
        MulOp->Type = MulOp->Circle;
        MulOp->Title = "op";
        if (token == "*")
        {
            match("*");
            MulOp->Details = "{*}";
        }
        if (token == "/")
        {
            match("/");
            MulOp->Details = "{/}";
        }
        return MulOp;
    }

    // factor -> (exp) | number | identifier
    //done
    Node* factor()
    {
        Node* Factor = new Node();
        Factor->Type = Factor->Circle;
        if (token == "(")
        {
            match("(");
            Factor = exp();
            match(")");

        }
        else if (token == "number")
        {
            Factor->Details = v[z].second;
            match("number");
            Factor->Title = "const";
        }
        else if (token == "identifier")
        {
            Factor->Details = v[z].second;
            match("identifier");
            Factor->Title = "id";
        }
        cout << "factor end" << endl;
        return Factor;
    }


};

#endif // PARSER_H
