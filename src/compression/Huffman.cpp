#include "Huffman.h"


void Huffman::Compress(void* stream) {
    
    ComputeDistribution(this->fd);
    AchieveNodes();
    SortAndMerge();
    CreateCodeTable(this->tree.front());
    PrintTable();
    PrintTableElem();
}

void Huffman::Decompress(void* stream) {

}

void Huffman::SortAndMerge(){

    while (this->tree.size() != 1) {
        
        this->tree.sort(HuffmanNode::OCompare());    // compare func or compare obj?

        HuffmanNode *L = this->tree.front();
        this->tree.pop_front();
        HuffmanNode *R = this->tree.front();
        this->tree.pop_front();

        HuffmanNode *P = new HuffmanNode(L, R);
        this->tree.push_back(P);

    }
}

void Huffman::CreateCodeTable(HuffmanNode *root) {
    if(root->left != nullptr) {
        this->code.push_back(0);
        CreateCodeTable(root->left);
    }

    if(root->right != nullptr) {
        this->code.push_back(1);
        CreateCodeTable(root->right);
    }

    if(root->left == nullptr && root->right == nullptr) {
        this->table[root->key] = this->code;
    }

    this->code.pop_back();
}

void Huffman::AchieveNodes(void){

    for(auto i = this->distribution.begin(); i != this->distribution.end(); i++) {
        HuffmanNode *n = new HuffmanNode(i->first, i->second);
        this->tree.push_back(n);
    }
}

void Huffman::FreeNodes(HuffmanNode *node) {
    if(node == nullptr) {
        return;
    }

    FreeNodes(node->left);
    FreeNodes(node->right);

    delete node;
}

uint8_t Huffman::PrintTableElem() {
    ssize_t ret = 0;
    uint8_t buf = 0;
    uint64_t fileOffset1 = 0, fileOffset2 = 0;

    int32_t fd2 = open("./out/output.cm", O_CREAT | O_RDWR);
    int16_t count = 0;

    while((ret = pread(fd, &buf, 1, fileOffset1)) != 0){
        if(ret == -1){
            if (errno == EINTR) continue; // handling some frequent interruptions
            return E_READ;
        }
        fileOffset1++;

        std::vector<bool> x = this->table[buf];

        buf = 0;
        
        for(int8_t bit = 0; bit < x.size(); bit++) {
            
            buf = buf | x[bit] << (7 - count);
            count++;

            if(count == 8) {
                count = 0;
                if((ret = pwrite(fd2, &buf, 1, fileOffset2)) != 0){
                    if(ret == -1){
                        if (errno != EINTR) return E_WRITE;
                    }
                }

                fileOffset2++;
            }

        }

    }

    close(fd2);

    return SUCCESS;
}

void Huffman::PrintTable() {
    std::cout << "Symbol Codes Table:\n";

    for(const auto &elem : this->table) {
        std::cout << elem.first << "\t";
        for(const auto &bit : elem.second){
            std::cout << bit;
        }
        std::cout << std::endl;
    }
}

uint8_t Huffman::ComputeDistribution(int32_t fd) {

    ssize_t ret = 0;
    uint8_t buf = 0;
    uint64_t fileOffset1 = 0;

    uint64_t mapCounter = 0;

    // slow positional read
    while((ret = pread(fd, &buf, 1, fileOffset1)) != 0){
        if(ret == -1){
            if (errno == EINTR) continue; // handling some frequent interruptions
            return E_READ;
        }
        fileOffset1++;

        if(!CheckMap(this->distribution, buf)) {
            distribution.emplace(buf, 1);
        } else {
            distribution[buf] = ++distribution[buf];
        }

    }

    lseek64(fd, 0, SEEK_SET);

    std::cout << std::endl;
    for( auto it = this->distribution.cbegin(); it != this->distribution.cend(); it++ ){

        printf(PRINTF_BINARY_PATTERN_INT8 "\t%d\n", PRINTF_BYTE_TO_BINARY_INT8((*it).first), (*it).second);

    }

    std::cout << "\nSize of map:\t" << this->distribution.size() << std::endl;
    std::cout << "Size of file:\t" << fileOffset1 << std::endl;

    return SUCCESS;
}

bool Huffman::CheckMap(std::map<uint8_t, uint32_t> m, uint8_t key) {

    return (m.find(key) == m.end()) ? false : true;
}
