#pragma once
#include <string>
#include <cstring>

#define CHIPID (uint32_t)(ESP.getEfuseMac() >> 24)
#define ESPMAC (Sprintf("%06x", CHIPID))
#define Sprintf(f, ...) ({ char* s; asprintf(&s, f, __VA_ARGS__); const std::string r = s; free(s); r; })
#define Stdprintf(f, ...) ({ char* s; asprintf(&s, f, __VA_ARGS__); const std::string r = s; free(s); r; })

std::string slugify(const std::string& text);
std::string slugify(const std::string& text);
std::string kebabify(const std::string& text);
std::string kebabify(const std::string& text);
std::string hexStr(const uint8_t *data, int len);
std::string hexStr(const char *data, int len);
std::string hexStr(const std::string& s);
std::string hexStrRev(const uint8_t *data, int len);
std::string hexStrRev(const char *data, int len);
std::string hexStrRev(const std::string &s);
bool hextostr(const std::string &hexStr, uint8_t* output, size_t len);
bool prefixExists(const std::string &prefixes, const std::string &s);
bool spurt(const std::string &fn, const std::string &content);
