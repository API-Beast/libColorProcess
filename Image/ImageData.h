#pragma once

#include "../Math/Vector2.h"

template<typename T>
struct ImageData
{
	T* data = nullptr;
	bool owns_data = false;
	int data_length = 0;
	Vec2i size = {0, 0};

	ImageData()=default;
	ImageData(const ImageData& other)=default;
	ImageData(ImageData&& other);
	~ImageData(){ clear(); };

	void allocate(int w, int h);
	void clear();
	constexpr int pixel_offset(){ return alignof(T); };
	int row_offset() const { return alignof(T) * size.x; };

	bool is_valid() const { return data != nullptr; };

	const T& operator[](int i) const{ return *(data+i); };
	      T& operator[](int i)      { return *(data+i); };

	T* begin(){ return data; };
	T* end()  { return data+data_length; };
	const T* begin() const { return data; };
	const T* end()   const { return data+data_length; };
};

template<typename T>
void ImageData<T>::allocate(int w, int h) 
{
	clear();
	owns_data = true;
	data_length = w*h;
	data = (T*)new alignas(T) char[data_length * sizeof(T)];
	size.x = w;
	size.y = h;
}

template<typename T>
void ImageData<T>::clear() 
{
	if(owns_data)
	{
		owns_data = false;
		delete[] data;
	}
}

template<typename T>
ImageData<T>::ImageData(ImageData<T>&& other) 
{
	size = other.size;
	owns_data = other.owns_data;
	data = other.data;
	other.owns_data = false;
	other.data = nullptr;
}