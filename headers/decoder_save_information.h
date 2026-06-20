#ifndef DECODER_SAVE_INFORMATION_H
#define DECODER_SAVE_INFORMATION_H

#include <vector>
#include <random>
#include <bitset>
#include <QString>
#include <QTextStream>
#include <QDebug>
#define MAX_SIZE 8

class Coder_save_information
{

    inline void adder_bits(std::bitset<MAX_SIZE>& mask, const int value_adder, int& pos_vec, int& pos_mask);
    QString txt;
    QTextStream& reader;

public:
    void shifr();
    explicit Coder_save_information(const QString& txt, QTextStream& reader);
};

class Decoder_save_information
{
    std::vector<bool> save_data;

    inline void getter_bits(std::bitset<MAX_SIZE>& mask, std::vector<bool>& vec, const int value_adder, int& pos_vec, int& pos_mask);
public:
    QString decode();
    explicit Decoder_save_information(std::vector<bool>&& save_data);
    void replace_data(std::vector<bool>&& save_data);
};

#endif // DECODER_SAVE_INFORMATION_H
