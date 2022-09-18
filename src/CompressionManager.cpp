#include "CompressionManager.h"
#include "../conf/conf.h"
#include "Common.h"


uint8_t CompressionManager::Initialize() 
{
    return 0;
}

off64_t CompressionManager::DoWork() {
    char buf[BUFSIZ];
    memset(buf, 0, sizeof(buf));
    ssize_t ret = 0;

    while ((ret = pread(params.fd1, buf, 
                        (params.amount > BUFSIZ) ? BUFSIZ : params.amount, 
                        params.fileOffset1)) != 0 && (params.amount - ret >= 0)) {
        if (ret == -1) {
            if (errno == EINTR) continue; // handling some frequent interruptions
            return -1;
        }
        params.fileOffset1 += ret;

        // TODO: compress data or decompress data with function pointer

        if((ret = pwrite(params.fd2, buf, ret, params.fileOffset2)) != 0){
            if (ret == -1) {
                if (errno == EINTR) continue; // handling some frequent interruptions
                return -1;
            }
        }

        params.fileOffset2 += ret;
        params.amount -= (int) ret;
    }

    return ret;
}

void CompressionManager::PrintHelpMessage(char* msg){
    if(msg != NULL) fprintf(stderr, "Error: %s, errno: %d\n", msg, errno);
    else {
        printf("\033[1m" "--- COMA ---\n" "\033[22m");
        printf("COmpression MAnager: an utility to (de)compress a file with specified parameters\n");
        printf("\033[1m" "Usage:" "\033[22m" " [OPTION]...\n"); 
        printf("\033[1m" " -i. --infile:""\033[22m" "    a file to operate with\n");
        printf("\033[1m" " -o, --outfile:""\033[22m" "   a file with the desired output\n");
        printf("\033[1m" " -m, --mode:""\033[22m" "      'c' for compression, 'd' for decompression\n");
    }
}

bool CompressionManager::ParseOptions(int argc, char** argv) {
    // extern int optind;
    extern char* optarg;
    char c = 0;

    while (1)
    {        
        int option_index = 0;
        
        static struct option long_options[] = {
            { "infile",    required_argument, 0, 'i' },
            { "outfile",   required_argument, 0, 'o' }, 
            { "mode",      required_argument, 0, 'm' },
            { "help",      no_argument,       0, 'h' },
            { 0,             0,               0,  0  },
        };
        c = getopt_long(argc, argv, "i:o:m:h", long_options, &option_index);
        if (c == -1)
            break; 
        
        switch (c)
        {
            case 'i':
                strcpy(opts.infile, optarg);
                break;
            case 'o':
                strcpy(opts.outfile, optarg);
                break;
            case 'm':
                if(strcmp(optarg, "c\0"))
                    opts.mode = 0;
                else if(strcmp(optarg, "d\0"))
                    opts.mode = 1;
                else {
                    PrintHelpMessage("Invalid mode");
                    return false; }
                break;
            case '?': case 'h':
            default:
                PrintHelpMessage(NULL);
                return false;
        }
    }

    return true;
};