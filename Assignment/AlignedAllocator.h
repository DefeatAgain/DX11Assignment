#pragma once
#include<new>
/*
	用于分配内存对齐对象，如XMMatrix
*/
template<typename T>
struct AlignedAllocator
{
	using value_type = T;

	const static unsigned AlignedBytes = 16U;

	AlignedAllocator()
	{
		CheckAligned<T>();
	}
	~AlignedAllocator() = default;

	template<typename U> AlignedAllocator(const AlignedAllocator<U>&) 
	{
		CheckAligned<U>();
	}
	template<typename U> AlignedAllocator(AlignedAllocator<U>&&)
	{
		CheckAligned<U>();
	}
	template<typename U> AlignedAllocator& operator=(const AlignedAllocator<U>&)
	{
		CheckAligned<U>();
		return *this;
	}
	template<typename U> AlignedAllocator& operator=(AlignedAllocator<U>&&)
	{
		CheckAligned<U>();
		return *this;
	}

	T* allocate(size_t n)
	{
		T* p = static_cast<T*>(::_aligned_malloc(n * sizeof(T), alignof(T)));
		if (!p)
			throw std::bad_alloc();
		return p;
	}

	void deallocate(T* p, size_t n) 
	{
		::_aligned_free(p);
	}
private:
	template<typename U> constexpr void CheckAligned() 
	{
		static_assert(alignof(U) == AlignedBytes, "Error: Type Not Aligned Of 16 Bytes!");
	}
};
