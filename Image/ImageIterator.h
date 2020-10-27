#pragma once

#include "../Math/Vector2.h"

template<typename T>
struct ImageIterator
{
	T* startPointer = nullptr;
	Vec2i min;
	Vec2i max;
	Vec2i offset;
	int curI = 0;

	ImageIterator() = default;
	ImageIterator(const ImageIterator& other, int i):startPointer(other.startPointer), min(other.min), max(other.max), offset(other.offset), curI(i){};
	ImageIterator(const ImageIterator& other):startPointer(other.startPointer), min(other.min), max(other.max), offset(other.offset), curI(other.curI){};
	int x() const { return min.x + curI%(max.x - min.x);};
	int y() const { return min.y + curI/(max.x - min.x);};
	int max_i() const { return (max.x - min.x) * (max.y - min.y); };
	ImageIterator begin() const { return {*this, curI}; };
	ImageIterator end() const { return   {*this, max_i()}; };
	T& operator*(){ return *reinterpret_cast<T*>(reinterpret_cast<char*>(startPointer) + x()*offset.x + y()*offset.y);};
	const T& operator*() const { return *reinterpret_cast<T*>(reinterpret_cast<char*>(startPointer) + x()*offset.x + y()*offset.y);};
	explicit operator bool() const{ return curI > 0 && curI < max_i();};
	bool operator< (const ImageIterator<T>& other) const{ return &(*other)  < &(**this); };
	bool operator> (const ImageIterator<T>& other) const{ return &(*other)  > &(**this); };
	bool operator<=(const ImageIterator<T>& other) const{ return &(*other) <= &(**this); };
	bool operator>=(const ImageIterator<T>& other) const{ return &(*other) >= &(**this); };
	bool operator==(const ImageIterator<T>& other) const{ return &(*other) == &(**this); };
	bool operator!=(const ImageIterator<T>& other) const{ return &(*other) != &(**this); };
	ImageIterator& operator++(){ curI++; return *this; };
	ImageIterator& operator--(){ curI--; return *this; };
	ImageIterator operator+(int i){ return {*this, curI + i}; };
	ImageIterator operator-(int i){ return {*this, curI - i}; };
};
