#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Функция циклического сдвига влево на n бит
uint32_t leftRotate(uint32_t value, int bits) 
{
    return (value << bits) | (value >> (32 - bits));
}

// Функция хэширования MD4
string md4_hash(const string& message) 
{
    // Шаг 1: Предварительная обработка (Padding)
    string paddedMessage = message;
    paddedMessage.push_back(0x80); // Добавляем байт 0x80 в конец сообщения
    while ((paddedMessage.length() * 8) % 512 != 448) 
    {
        paddedMessage.push_back(0x00); // Дополняем нулевыми байтами до кратности 512 битам
    }

    uint64_t messageLength = message.length() * 8; // Длина исходного сообщения в битах
    for (int i = 0; i < 8; i++) 
    {
        paddedMessage.push_back((messageLength >> (i * 8)) & 0xFF); // Добавляем байты, представляющие длину сообщения
    }

    // Шаг 2: Инициализация регистров (Initialization)
    uint32_t A = 0x67452301;
    uint32_t B = 0xEFCDAB89;
    uint32_t C = 0x98BADCFE;
    uint32_t D = 0x10325476;

    // Шаг 3: Обработка блоков сообщения (Message Processing)
    for (size_t i = 0; i < paddedMessage.length(); i += 64) 
    {
        vector<uint32_t> X(16);

        // Разделение блока на 16 32-битных слов
        for (int j = 0; j < 16; j++) 
        {
            X[j] = static_cast<uint32_t>(paddedMessage[i + j * 4]) |
                (static_cast<uint32_t>(paddedMessage[i + j * 4 + 1]) << 8) |
                (static_cast<uint32_t>(paddedMessage[i + j * 4 + 2]) << 16) |
                (static_cast<uint32_t>(paddedMessage[i + j * 4 + 3]) << 24);
        }

        // Сохранение начальных значений регистров
        uint32_t AA = A;
        uint32_t BB = B;
        uint32_t CC = C;
        uint32_t DD = D;

        // Шаг 4: Раунды (Rounds)
        for (int j = 0; j < 64; j++) 
        {
            uint32_t F, g;

            if (j < 16) 
            {
                F = (B & C) | ((~B) & D);
                g = j;
            }
            else if (j < 32) 
            {
                F = (B & C) | (B & D) | (C & D);
                g = (5 * j + 1) % 16;
            }
            else if (j < 48) 
            {
                F = B ^ C ^ D;
                g = (3 * j + 5) % 16;
            }
            else 
            {
                F = C ^ (B | (~D));
                g = (7 * j) % 16;
            }

            uint32_t temp = D;
            D = C;
            C = B;
            B = B + leftRotate((A + F + X[g]), 3);
            A = temp;
        }

        // Обновление значений регистров
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    // Шаг 5: Формирование хэша (Output)
    ostringstream oss;
    oss << hex << setfill('0');
    oss << setw(8) << A
        << setw(8) << B
        << setw(8) << C
        << setw(8) << D;

    return oss.str();
}

int main() 
{
    string message = "aaaa";
    string hash = md4_hash(message);

    cout << "Message: " << message << endl;
    cout << "MD4 Hash: " << hash << endl;

    return 0;
}