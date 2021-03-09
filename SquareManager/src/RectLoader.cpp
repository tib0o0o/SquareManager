#pragma once
#include "RectLoader.h"
#include "RectTypes.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>
#include <algorithm>  
#include <cmath>
// std::sort

struct less_than_key
{
    inline bool operator() (const Rect& struct1, const Rect& struct2)
    {
        return (struct1.w > struct2.w);
    }
};

RectLoader::RectLoader(const std::string& filePath)
{
    RectLoader::FilePath = filePath;
    RectLoader::Rects.clear();
    RectLoader::Spaces.clear();
    RectLoader::SquareMinWidth = 0;
}
std::vector<Rect> RectLoader::load()
{
    std::ifstream file(RectLoader::FilePath);
    Rects.clear();
    Spaces.clear();
    if (file)
    {
        std::cout << "file good\n";
        file.seekg(0, std::ios::end);
        const auto size = file.tellg();
        std::string str(size, ' ');
        file.seekg(0);
        file.read(&str[0], size);
        file.close();
        for (int i = 0; (i + 4) <= size ; i+=6)
        {
            Rects.push_back({ (int)str[i + 2] - 48, (int)str[i + 4] - 48, 0, 0, (int)str[i] - 48 });
        }
    }
    else
    {
        std::cout << "file problem\n";
    }
    return Rects;
}

std::vector<Rect> RectLoader::backe()
{
    bool result = false;
    if (RectLoader::Rects.size() != 0)
    {
        //calculate the minimum size of the map
        int area = 0;
        RectLoader::SquareMinWidth = 0;

        for (Rect box : RectLoader::Rects) {
            area += box.w * box.h;
            if (box.w > RectLoader::SquareMinWidth)
            {
                RectLoader::SquareMinWidth = box.w;
            }
        }
        RectLoader::SquareMinWidth = std::max((int)std::ceil(std::sqrt(area)), RectLoader::SquareMinWidth);

        // sort the boxes for insertion by height, descending
        std::sort(RectLoader::Rects.begin(), RectLoader::Rects.end(), less_than_key());
        
        int startWidth = RectLoader::SquareMinWidth;
       
        // start with a single empty space, unbounded at the bottom
        RectLoader::Spaces.push_back( { startWidth, INT_MAX, 0, 0 } );

        int width = 0;
        int height = 0;


        for (Rect box : RectLoader::Rects) {
            // look through spaces backwards so that we check smaller spaces first
            for (int i = RectLoader::Spaces.size() - 1; i >= 0; i--) {
                Space newSpace = RectLoader::Spaces.at(i);
               
                if (box.w > newSpace.w || box.h > newSpace.h) continue;

                // found the space; add the box to its top-left corner
                // |-------|-------|
                // |  box  |       |
                // |_______|       |
                // |         space |
                // |_______________|
                box.x = newSpace.x;
                box.y = newSpace.y;

                height = std::max(box.y + box.h, height);
                width = std::max(box.x + box.w, width);

                if (box.w == newSpace.w && box.h == newSpace.h) {
                    // space matches the box exactly; remove it
                    Space last = RectLoader::Spaces[RectLoader::Spaces.size() - 1];
                    RectLoader::Spaces.pop_back();
                    if (i < (int)RectLoader::Spaces.size()) RectLoader::Spaces.push_back(last);

                }
                else if (box.h == newSpace.h) {
                    // space matches the box height
                    // |-------|---------------|
                    // |  box  | updated space |
                    // |_______|_______________|
                    newSpace.x += box.w;
                    newSpace.w -= box.w;

                }
                else if (box.w == newSpace.w) {
                    // space matches the box width
                    // |---------------|
                    // |      box      |
                    // |_______________|
                    // | updated space |
                    // |_______________|
                    newSpace.y += box.h;
                    newSpace.h-= box.h;

                }
                else {
                    // otherwise the box splits the space into two spaces
                    // |-------|-----------|
                    // |  box  | new space |
                    // |_______|___________|
                    // | updated space     |
                    // |___________________|
                    RectLoader::Spaces.push_back({
                        newSpace.w - box.w,
                        box.h,
                        newSpace.x + box.w,
                        newSpace.y
                        });
                    newSpace.y += box.h;
                    newSpace.h -= box.h;
                }
                RectLoader::Spaces[i] = newSpace;
                break;
            }
        }
        RectLoader::SquareMinWidth = std::max(width, height);
        std::cout << "SquareMinWidth" << SquareMinWidth << "\n";
    }
    return RectLoader::Rects;
}

