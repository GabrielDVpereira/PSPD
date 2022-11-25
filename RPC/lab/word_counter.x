struct params {
        char data<>;
        char word<>;
};


struct result {
        int occurrences;
        int count;
};


program PROG {
        version VERSAO {
                result COUNT_WORD(params) = 1;
        } = 100;
} = 55555555;
