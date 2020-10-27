#pragma once

#include <iterator>

template<typename T>
struct IteratorRange
{
	T start;
	T stop;
	T begin() const{ return start; };
	T end() const{ return stop; };
};

template<typename T>
struct ImageIterator : public std::iterator<std::random_access_iterator_tag, T>
{
	T* ptr = nullptr;
	T* row_start = nullptr;
	ptrdiff_t row_offset = 0;
	int max_x = 0;
	constexpr T& operator*(){ return *ptr; };
	constexpr ImageIterator& operator+=(ptrdiff_t i)
	{
		T* new_ptr = ptr + i;
		int rows_skipped = (new_ptr - row_start) / max_x;
		int new_x = (new_ptr - row_start) % max_x;
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
		return y_diff * max_x + x_diff;
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
	IteratorRange<ImageIterator<T> > rectangle(ImageData<T>& data, int x, int y, int w, int h)
	{
		IteratorRange<ImageIterator<T>> retVal;
		x = std::max(std::min(x, data.size.x), 0);
		w = std::max(std::min(w, data.size.x - x), 0);

		retVal.start.row_start = data.row(y);
		retVal.start.ptr = retVal.start.row_start + x;
		retVal.start.row_offset = data.row_offset();
		retVal.start.max_x = x+w;

		retVal.stop.row_start = data.row(y+h);
		retVal.stop.ptr = retVal.stop.row_start + x + w;
		retVal.stop.row_offset = data.row_offset();
		retVal.stop.max_x = x+w;

		return retVal;
	};
}