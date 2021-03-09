#pragma once
#include <string>
#include <vector>
#include "RectTypes.h"


class RectLoader
{
private:
    std::string FilePath;
    std::vector<Rect> Rects;
    std::vector<Space> Spaces;
    int SquareMinWidth;
public:
    RectLoader(const std::string& filePath);
    std::vector<Rect> load();
    std::vector<Rect> backe();
};