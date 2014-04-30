#ifndef _FILE_CONTENT_HPP
#define _FILE_CONTENT_HPP

#include <string>

namespace huffman_comp
{

//determine the content of the compressed file in order
class file_content
{
    public:
        size_t huffman_tree_size; //size of encoded huffman tree stored in the compressed file (in bits)
        size_t leaves_size; //size of leaves stored in the compressed file (in bytes)
        size_t encoded_filesize; //size of the part of the compressed file that stores encoded data (in bits)
        std::string encoded_huffman_tree;//a binary string (different from the compressed file where each char is actually stored in a bit)
        std::string leaves;//labels of leaves of huffman tree, each leaf is a unique char from the original file
        std::string encoded_file;//a binary string (different from the compressed file where each char is actually stored in a bit)

        file_content() : huffman_tree_size(0), leaves_size(0), encoded_filesize(0),
                                encoded_huffman_tree(""), leaves(""), encoded_file("") {}
        std::string read_next(std::ifstream &, int);
        //read from an input file and store data in the object
        void read_content(const std::string &);
        //read data from the object and store it in an output file
        void write_content(const std::string &);

        void print();

//    friend class encoder;
//    friend class decoder;
};

} // namespace huffman_comp


#endif //_FILE_CONTENT_HPP
