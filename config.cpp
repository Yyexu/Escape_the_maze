#include "config.h"
#include <fstream>
#include <cstdlib>

// 定义并初始化
int g_ROW = ROW;   // 先把宏值拿过来
int g_COL = COL;
int START_X = 0;
int START_Y = 0;
int END_X = COL - 1;
int END_Y = ROW - 1;
bool USE_CUSTOM_MAP = false;   // 默认关闭
int Skin = 0;

static std::string trim(const std::string& s) {
    size_t f = s.find_first_not_of(" \t\r\n");
    if (f == std::string::npos) return "";
    size_t l = s.find_last_not_of(" \t\r\n");
    return s.substr(f, l - f + 1);
}

void initConfig() {
    std::ifstream fs("settings.txt");
    if (!fs) return;

    std::string line;
    while (std::getline(fs, line)) {
        line = trim(line);
        if (line.empty() || line.front() == '#' || line.front() == '[') continue;
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));

        if (key == "Difficulty") {
            int d = std::atoi(val.c_str());
            switch (d) {
            case 1: g_ROW = g_COL = 10; break;
            case 2: g_ROW = g_COL = 20; break;
            case 3: g_ROW = g_COL = 30; break;
            }
        }
        else if (key == "StartX") START_X = std::atoi(val.c_str());
        else if (key == "StartY") START_Y = std::atoi(val.c_str());
        else if (key == "EndX")   END_X = std::atoi(val.c_str());
        else if (key == "EndY")   END_Y = std::atoi(val.c_str());
        else if (key == "UseCustomMap") USE_CUSTOM_MAP = (std::atoi(val.c_str()) != 0);
        else if (key == "Skin")  Skin = std::atoi(val.c_str());
    }

    // 合法性 clamp
    if (START_X < 0) START_X = 0; if (START_X >= g_COL) START_X = g_COL - 1;
    if (START_Y < 0) START_Y = 0; if (START_Y >= g_ROW) START_Y = g_ROW - 1;
    if (END_X < 0) END_X = 0; if (END_X >= g_COL) END_X = g_COL - 1;
    if (END_Y < 0) END_Y = 0; if (END_Y >= g_ROW) END_Y = g_ROW - 1;
}

std::vector<std::vector<bool>> loadCustomMap() {
    std::vector<std::vector<bool>> wall;     // true=墙
    std::ifstream fs("settings.txt");
    if (!fs) return wall;

    std::string line;
    bool inSection = false;
    while (std::getline(fs, line)) {
        if (line.find("[CustomMap]") != std::string::npos) { inSection = true; continue; }
        if (!inSection) continue;
        if (line.empty() || line[0] == '[') break;
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        if (line.empty()) continue;

        std::vector<bool> row;
        for (char c : line) row.push_back(c == '1');
        wall.push_back(row);
    }

    if (!wall.empty()) {
        g_ROW = wall.size();
        g_COL = wall[0].size();
    }
    return wall;
}