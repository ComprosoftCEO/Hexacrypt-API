//Nice command line program that can accept parameters and run
//  tests on the various Hexacrypt algorithms

#include <Hexacrypt-API.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef const char* (*HAction_Fun_t)(int,char**);
typedef struct {
    const char* arg;          //Command line argument (-e, -d, -E, -D, etc...)
    const char* params;       //What are the parameters needed
    const char* description;  //What does this command line do
    HAction_Fun_t fun;  //Function to run with this argument
} HAction_t, *pHAction_t;

const char* do_hexacrypt_encrypt(int argc, char** argv);
const char* do_hexacrypt_decrypt(int argc, char** argv);
const char* do_hexacrypt_plus_encrypt(int argc, char** argv);
const char* do_hexacrypt_plus_decrypt(int argc, char** argv);

static const HAction_t ALL_ACTIONS[] = {
    {"-e",  "<msg> <key>",     "Encrypt with Hexacrypt",   do_hexacrypt_encrypt},
    {"-d",  "<msg> <key>",     "Decrypt with Hexacrypt",   do_hexacrypt_decrypt},
    {"-E",  "<msg> <key> [r]", "Encrypt with Hexacrypt Plus ([r] rounds, default=10)", do_hexacrypt_plus_encrypt},
    {"-D",  "<msg> <key> [r]", "Decrypt with Hexacrypt Plus ([r] rounds, default=10)", do_hexacrypt_plus_decrypt}
};



static void show_help(const char* appname) {
    printf("Usage: %s (ALGORITHM) (OPTIONS)\n\n",appname);

    size_t i;
    printf("Algorithms:\n");
    for (i = 0; i < (sizeof(ALL_ACTIONS) / sizeof(ALL_ACTIONS[i])); ++i) {
        const HAction_t* hact = ALL_ACTIONS+i;
        printf("\t%s %s = %s\n",hact->arg, hact->params, hact->description);
    }
}


int main(int argc, char** argv) {

    int i;
    const char* result = NULL;

    //Show Help
    if (argc < 2 || !strcmp(argv[1],"--help")) {
        show_help("Hexacrypt-API");
        return 0;
    }

    for (i = 0; i < (sizeof(ALL_ACTIONS) / sizeof(ALL_ACTIONS[0])); ++i) {
        if (!strcmp(argv[1],ALL_ACTIONS[i].arg)) {
            result = ALL_ACTIONS[i].fun(argc,argv);
        }
    }

    if (!result) {
        printf("Hexacrypt: Unrecognized parameter %s\n",argv[1]);
        printf("Hexacrypt: Try '%s --help' for more information\n",argv[0]);
        return 1;
    }

    if (result == (const char*) -1) {
        return 1;
    }

    printf("%s\n",result);
    return 0;
}


const char* do_hexacrypt_encrypt(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s -e <msg> <key>\n",argv[0]);
        return (const char*) -1;
    }

    return Hexacrypt_Encrypt(argv[2],argv[3]);
}


const char* do_hexacrypt_decrypt(int argc, char** argv) {
     if (argc < 4) {
        printf("Usage: %s -d <msg> <key>\n",argv[0]);
        return (const char*) -1;
    }

    return Hexacrypt_Decrypt(argv[2],argv[3]);
}


const char* do_hexacrypt_plus_encrypt(int argc, char** argv) {
    uint32_t rounds = 10;
    if (argc < 4) {
        printf("Usage: %s -E <msg> <key> [r]\n",argv[0]);
        return (const char*) -1;
    }

    if (argc >= 5) {rounds = atol(argv[4]);}

    return HexacryptPlus_Encrypt(argv[2],argv[3],rounds);
}



const char* do_hexacrypt_plus_decrypt(int argc, char** argv) {
    uint32_t rounds = 10;
    if (argc < 4) {
        printf("Usage: %s -D <msg> <key> [r]\n",argv[0]);
        return (const char*) -1;
    }

    if (argc >= 5) {rounds = atol(argv[4]);}

    return HexacryptPlus_Decrypt(argv[2],argv[3],rounds);
}
