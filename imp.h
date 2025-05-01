namespace imp{
    bool isDigit(char input) {
        return (input<=57 &&input>=48);
    }
    bool isLetter(char input) {
        return (input >= 97 && input <=122);
    }
    bool isOperator(char input) {
        return (input=='+'||input=='-'||input=='*'||input=='/');
    }
    bool isSemicolon(char input) {
        return (input == ';');
    }
    bool isEqual(char input) {
        return (input == '=');
    }
    bool isDot(char input) {
        return (input =='.');
    }
    bool isFlag(char input) {
        return (input =='!');
    }
    bool isAmbersand(char input) {
        return (input =='&');
    }
    bool isBackslash(char input) {
        return (input =='\\');
    }
    bool isFrontParen(char input) {
        return (input =='(');
    }
    bool isBackParen(char input) {
        return (input ==')');
    }
    bool isA(char input) {
        return (input=='A');
    }
    bool isB(char input) {
        return (input=='B');
    }
    bool isC(char input) {
        return (input=='C');
    }
    bool isT(char input) {
        return (input=='T');
    }
}