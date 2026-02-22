#include "NumberOfRectangles.hpp"


NumberOfRectangles::NumberOfRectangles(uint16_t value) : value(value)
{
}

std::string NumberOfRectangles::getInfo()
{
    return std::string();
}

std::ostream& operator<<(std::ostream& os, const NumberOfRectangles &numberOfRectangles)
{
    os <<"numberOfRectangles: "<<numberOfRectangles.value<<std::endl;
    return os;
}
