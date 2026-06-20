#include "decoder_save_information.h"

Coder_save_information::Coder_save_information(const QString& txt, QTextStream& reader) : txt(txt), reader(reader)
{}
Decoder_save_information::Decoder_save_information(std::vector<bool>&& save_data) : save_data(std::move(save_data))
{}

void Coder_save_information::shifr()
{
    /*
    encodes information into bits. each character occupies 8 bits + a key of 8. The total size is (n elements + 1 ) * 8.
    */
    using namespace std;
    mt19937 gen{random_device()()};
    uniform_int_distribution<int> disttib(0, 1);
    bitset<MAX_SIZE> mask_bit(0);
    vector<bitset<MAX_SIZE>> vec_bit;

    for(int i = 0; i < MAX_SIZE; ++i)
    {
        mask_bit[i] = disttib(gen);
    }

    for(qsizetype i = 0; i < txt.length(); ++i){
        vec_bit.emplace_back(bitset<MAX_SIZE>(static_cast<uint32_t>(txt[i].unicode())));
    }
    vec_bit[0] ^= mask_bit;
    for(size_t i = 1; i < vec_bit.size(); ++i)
    {
        vec_bit[i] ^= vec_bit[0] ^ mask_bit;
    }

    int pos_save_data = 0, pos_vec_bit = 0, pos_mask_bit = 0;

    int vvec_bit_sz = vec_bit.size();
    while(true)
    {
        if(vvec_bit_sz < 8 && pos_mask_bit<8)
        {
            adder_bits(mask_bit, (MAX_SIZE - pos_mask_bit + pos_mask_bit)/vvec_bit_sz+1, pos_save_data, pos_mask_bit);
        }
        else if(pos_mask_bit<8) {pos_save_data++;  reader << mask_bit[pos_mask_bit++];}

        if(!(pos_save_data <= vvec_bit_sz * MAX_SIZE + MAX_SIZE)) break;

        for(int i = 0; i < MAX_SIZE; ++i){
            pos_save_data++; reader << vec_bit[pos_vec_bit][i];
        }
        pos_vec_bit++;
    }
    reader << '\n';
}
// переделать алгоритм шифровки
inline void Coder_save_information::adder_bits(std::bitset<MAX_SIZE>& mask, const int value_adder, int& pos_vec, int& pos_mask)
{
    for(int i = 0;  i < value_adder; ++i, ++pos_mask, pos_vec++)
    {
        if(pos_mask > 7) return;
        reader << mask[pos_mask];
    }
}
inline void Decoder_save_information::getter_bits(std::bitset<MAX_SIZE>& mask, std::vector<bool>& vec, const int value_adder, int& pos_vec, int& pos_mask)
{
    for(int i = 0;  i < value_adder; ++i, ++pos_mask, pos_vec++)
    {
        if(pos_mask > 7) return;
        mask[pos_mask] = vec[pos_vec];
    }
}

QString Decoder_save_information::decode()
{
    /*
     decodes information into bits. each character occupies 8 bits + a key of 8. The total size is (n elements + 1 ) * 8.
     */

    using namespace std;
    int pos_save_data = 0, pos_vec_bit = 0, pos_mask_bit = 0;
    int vvec_bit_sz = (save_data.size() - 8) / 8;

    bitset<MAX_SIZE> mask_bit;
    vector<bitset<MAX_SIZE>> vec_chars;
    for(int i = 0; i < vvec_bit_sz-1; ++i) vec_chars.emplace_back(bitset<MAX_SIZE>());
    while(true)
    {
        if(vvec_bit_sz < 8 && pos_mask_bit<8)
        {
            getter_bits(mask_bit, save_data, (MAX_SIZE - pos_mask_bit + pos_mask_bit)/vvec_bit_sz+1, pos_save_data, pos_mask_bit);
        }
        else if(pos_mask_bit<8) mask_bit[pos_mask_bit++] = save_data[pos_save_data++];

        if(!(pos_save_data < vvec_bit_sz * MAX_SIZE + MAX_SIZE)
            ||(pos_save_data == vvec_bit_sz * MAX_SIZE + MAX_SIZE) ) break;

        for(int i = 0; i < MAX_SIZE; ++i){
            vec_chars[pos_vec_bit][i] = save_data[pos_save_data++];
        }
        pos_vec_bit++;
    }
    for(int i = vec_chars.size(); i != 0; --i)
    {
        //decode
        vec_chars[i] ^= vec_chars[0] ^ mask_bit;
    }
    vec_chars[0] ^= mask_bit;
    QString result_txt;
    for(auto& c : vec_chars)
    {
        result_txt.push_back(static_cast<char>(c.to_ulong()));
    }
    qDebug() << result_txt;

    return result_txt;
}
void Decoder_save_information::replace_data(std::vector<bool>&& save_data)
{
    this->save_data = std::move(save_data);
}
