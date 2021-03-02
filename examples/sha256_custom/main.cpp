#include <bits/stdc++.h>
#include <sys/time.h>
#include "sha256_custom.h"

// #include <openssl/sha.h>
// std::string sha256(const std::string &str) {
//     char buf[5];
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, str.c_str(), str.size());
//     SHA256_Final(hash, &sha256);
//     std::stringstream ss;
//     for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//         sprintf(buf, "%02x", hash[i]);
//         ss << buf;
//     }
//     return ss.str();
// }

std::string sha256(const std::string& str) {
    SHA256 sha;
    sha.update(str);
    uint8_t * digest = sha.digest();
    std::string ret = SHA256::toString(digest);
    delete[] digest;
    return ret;
}

std::string randstr(const int len) {
    // srand(time(NULL));
    std::stringstream ss;
    for (int i = 0; i < len; ++i) {
        switch ((rand() % 3)) {
        case 1:
            ss << (char)('A' + rand() % 26);
            break;
        case 2:
            ss << (char)('a' + rand() % 26);
            break;
        default:
            ss << (char)('0' + rand() % 10);
            break;
        }
    }
    return ss.str();
}

// int main() {
//     std::string x  = "hello world"; // b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9
//     std::cout << sha256(x) << std::endl;   
// }

int main(int argc, char *argv[]) {
    int n = 20 * (1 << 20); 
    int numreps = 100;
    struct timeval tv1, tv2;

    // initialize the string
    if (argc >= 2) {
        n = atoi(argv[1]);
    }
    if (argc >= 3) {
        numreps = atoi(argv[2]);
    }

    std::string x = randstr(n);

    // multiply calculation
    printf("Multiply calculation %d times...\n", numreps);
    gettimeofday(&tv1, NULL);
    for (int i = 0; i < numreps; i++) {
        sha256(x);
    }
    gettimeofday(&tv2, NULL);
    double elapsed =
        (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("Total time = %lf, %d runs, avg time = %lf\n", elapsed, numreps, elapsed / numreps);
    return 0;
}