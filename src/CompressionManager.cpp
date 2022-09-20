#include "CompressionManager.h"

uint8_t CompressionManager::Initialize() 
{   
    int fd1 = open(opts.infile, O_RDONLY);
    if(fd1 < 0)
    {
        PrintHelpMessage((char*)"Cannot open input file");
        return 1;
    }

    int fd2 = open(opts.outfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd2 < 0)
    {
        PrintHelpMessage((char*)"Cannot open output file");
        return 1;
    }

    struct stat st;
    if(fstat(fd1, &st) < 0)
    {
        PrintHelpMessage((char*)"Cannot stat input file");
        return 1;
    }

    this->input = new FileStream(fd1, st.st_size);
    this->output = new FileStream(fd2, 0);

    opts.action = opts.mode ? &RLE<BUF_SIZE>::Compression : &RLE<BUF_SIZE>::Decompression;

    return 0;
}

ssize_t CompressionManager::DoWork() 
{
    return opts.action(input, output);
}


void CompressionManager::PrintHelpMessage(char* msg){
    if(msg != NULL) fprintf(stderr, "Error: %s, errno: %d (%s)\n", msg, errno, strerror(errno));
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
                strcat(opts.infile, optarg);
                break;
            case 'o':
                strcat(opts.outfile, optarg);
                break;
            case 'm':
                if(strcmp(optarg, "c\0"))
                    opts.mode = 0;
                else if(strcmp(optarg, "d\0"))
                    opts.mode = 1;
                else {
                    PrintHelpMessage((char*)"Invalid mode");
                    return false; 
                    }
                break;
            case '?': case 'h':
            default:
                PrintHelpMessage(NULL);
                return false;
        }
    }

    return true;
};