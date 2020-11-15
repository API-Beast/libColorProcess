#pragma once

#include <iterator>

template<typename T>
struct IteratorRange
{
	T start;
	T stop;
	T begin() const{ return start; };
	T end() const{ return stop; };

	bool operator==(IteratorRange other) const
	{
		return other.start == this->start && other.stop == this->stop;
	};
};

template<typename T>
struct ImageIterator : public std::iterator<std::random_access_iterator_tag, T>
{
	T* ptr = nullptr;
	T* row_start = nullptr;
	ptrdiff_t row_offset = 0;
	int width = 0;
	constexpr T& operator*(){ return *ptr; };
	constexpr ImageIterator& operator+=(ptrdiff_t i)
	{
		constexpr auto euc_mod = [](int a, int b) constexpr{ return a < 0 ? (a % b) + b     : a % b; };
		constexpr auto euc_div = [](int a, int b) constexpr{ return a < 0 ? (a - b + 1) / b : a / b; };

		T* new_ptr = ptr + i;
		auto diff = new_ptr - row_start;
		int rows_skipped = euc_div(diff, width);
		int new_x        = euc_mod(diff, width);
		row_start += rows_skipped * row_offset;
		ptr = row_start + new_x;
		return *this;
	};
	constexpr ImageIterator& operator-=(ptrdiff_t i){ return operator+=(-i); }
	constexpr ImageIterator& operator++(){ return operator+=( 1); };
	constexpr ImageIterator& operator--(){ return operator+=(-1); };
	constexpr ImageIterator operator++(int){ auto copy = *this; operator+=( 1); return copy; };
	constexpr ImageIterator operator--(int){ auto copy = *this; operator+=(-1); return copy; };

	ImageIterator operator+(ptrdiff_t i)
	{
		ImageIterator retVal(*this);
		retVal += i;
		return retVal;
	};
	ImageIterator operator-(ptrdiff_t i){ return operator+(-i);}

	ptrdiff_t operator-(const ImageIterator<T>& other)
	{
		ptrdiff_t x_diff = (ptr - row_start) - (other.ptr - other.row_start);
		ptrdiff_t y_diff = (row_start - other.row_start) / row_offset;
		return y_diff * width + x_diff;
	}

	constexpr bool operator< (const ImageIterator<T>& other) const{ return ptr  < other.ptr; };
	constexpr bool operator> (const ImageIterator<T>& other) const{ return ptr  > other.ptr; };
	constexpr bool operator<=(const ImageIterator<T>& other) const{ return ptr <= other.ptr; };
	constexpr bool operator>=(const ImageIterator<T>& other) const{ return ptr >= other.ptr; };
	constexpr bool operator==(const ImageIterator<T>& other) const{ return ptr == other.ptr; };
	constexpr bool operator!=(const ImageIterator<T>& other) const{ return ptr != other.ptr; };
};

namespace Iterate
{
	template<typename T>
	IteratorRange<ImageIterator<T> > region(ImageData<T>& data, int x, int y, int x2, int y2)
	{
		IteratorRange<ImageIterator<T>> retVal;
		if(x > x2){ std::swap(x, x2); };
		if(y > y2){ std::swap(y, y2); };

		x  = std::max(std::min( x, data.size.x), 0);
		y  = std::max(std::min( y, data.size.y), 0);
		x2 = std::max(std::min(x2, data.size.x), 0);
		y2 = std::max(std::min(y2, data.size.y), 0);
		int w = x2 - x;
		int h = y2 - y;

		if(w == 0 || h == 0)
		{
			w = 1;
			retVal.start.row_start = data.row(y) + x;
			retVal.start.ptr = retVal.start.row_start;
			retVal.start.row_offset = data.row_offset();
			retVal.start.width = w;

			retVal.stop = retVal.start;

			return retVal;
		}

		retVal.start.row_start = data.row(y) + x;
		retVal.start.ptr = retVal.start.row_start;
		retVal.start.row_offset = data.row_offset();
		retVal.start.width = w;

		retVal.stop.row_start = data.row(y + h - 1) + x;
		retVal.stop.ptr = retVal.stop.row_start + w - 1;
		retVal.stop.row_offset = data.row_offset();
		retVal.stop.width = w;
		retVal.stop++;

		return retVal;
	};

	template<typename T>
	IteratorRange<ImageIterator<T> > region(ImageData<T>& data, Vec2i start_pos, Vec2i end_pos)
	{
		return region(data, start_pos.x, start_pos.y, end_pos.x, end_pos.y);
	};

	template<typename T>
	IteratorRange<ImageIterator<T> > rectangle(ImageData<T>& data, int x, int y, int w, int h)
	{
		return region(data, x, y, x+w, y+h);
	};

	template<typename T>
	IteratorRange<ImageIterator<T> > rectangle(ImageData<T>& data, Vec2i pos, Vec2i size)
	{
		return region(data, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h);
	};

	template<typename T>
	std::vector< IteratorRange< ImageIterator<T> > > chunks(ImageData<T>& data, int w, int h)
	{
		std::vector< IteratorRange< ImageIterator<T> > > retVal;
		for(int y = 0; y < data.size.y; y += h)
		for(int x = 0; x < data.size.x; x += w)
			retVal.push_back(rectangle(data, x, y, w, h));
		return retVal;
	};

	template<typename T>
	std::vector< IteratorRange< ImageIterator<T> > > chunks(ImageData<T>& data, Vec2i size)
	{
		return chunks(data, size.x, size.y);
	};
}