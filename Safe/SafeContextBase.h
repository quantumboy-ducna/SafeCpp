/// <summary>
///		Legal & Licensing Information
/// </summary>
/// <remarks>
///		Required Notice: Copyright©2026, Nguyễn Anh Đức (workofduc@gmail.com). All Rights Reserved.
///
///		DUAL-LICENSING MODEL:
///		This software is dual-licensed to accommodate both open-source development and proprietary commercial use.
///
///		OPEN-SOURCE TRACK (GPLv3):
///		This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
///
///		COMMERCIAL TRACK:
///		For commercial entities wishing to embed this software into proprietary, closed-source software, a separate commercial license is required. This grants the legal right to use the library without being bound by the GPLv3 copyleft requirements.
///
///		CONTACT:
///		For commercial licensing inquiries, pricing, or to obtain a proprietary license agreement, please contact: workofduc@gmail.com
/// </remarks>

#pragma once

/** Inclusion(s) of C++ standard library header file(s).**/
#include <concepts>
#include <type_traits>
#include <typeinfo>
#include <vector>

/** Inclusion(s) of project's C++ header file(s).**/
#include "SafeNamespace.h"
#include "SafeTrade.h"


/** Main code.**/

/// <summary>
///		C++ namespace: `Safe`.
/// </summary>
namespace Safe
{
	/// <summary>
	///		C++ class: `SafeContextBase`.
	/// </summary>
	class SafeContextBase
	{
	private:
		/// <summary>
		///		C++ class: `SafeMemoryManager`.
		/// </summary>
		class SafeMemoryManager;

		/// <summary>
		///		C++ functional pointer type: `DefaultConstructionInvoker`.
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		typedef void (*DefaultConstructionInvoker)(SafeContextBase* const instancePointer);

		/// <summary>
		///		C++ functional pointer type: `CopyConstructionInvoker`.
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <param name="otherInstancePointer"></param>
		/// <returns>void</returns>
		typedef void (*CopyConstructionInvoker)(SafeContextBase* const instancePointer,const SafeContextBase* const otherInstancePointer);


		bool life;

	public:
		/// <summary>
		///		C++ class: `SafeMemoryChunk`.
		/// </summary>
		/// <typeparam name="GenericTypeOfSafeContextDerivative"></typeparam>
		template<typename GenericTypeOfSafeContextDerivative> requires std::equality_comparable<GenericTypeOfSafeContextDerivative> class SafeMemoryChunk;

		/// <summary>
		///		Constructor of `SafeContextBase`.
		/// </summary>
		explicit SafeContextBase();

	private:
		/// <summary>
		///		Constructor of `SafeContextBase`.
		/// </summary>
		/// <param name="classifcation"></param>
		explicit SafeContextBase(const bool& classifcation);

	protected:
		/// <summary>
		///		Copy constructor of `SafeContextBase`.
		/// </summary>
		/// <param name="other"></param>
		explicit SafeContextBase(const SafeContextBase& other) noexcept(false);

		/// <summary>
		///		Move constructor of `SafeContextBase`.
		/// </summary>
		/// <param name="classifcation"></param>
		explicit SafeContextBase(SafeContextBase&& other) noexcept(false);

		/// <summary>
		///		Destructor of `SafeContextBase`.
		/// </summary>
		/// <exception cref="SafeContextException"/>
		virtual ~SafeContextBase() noexcept(false);

		/// <summary>
		///		static
		///		operator new()
		/// </summary>
		/// <param name="memorySize"></param>
		/// <returns>void*</returns>
		static void* operator new(std::size_t memorySize);

		/// <summary>
		///		static
		///		noexcept
		///		operator delete()
		/// </summary>
		/// <param name="memoryPointer"></param>
		/// <returns>void</returns>
		static void operator delete(void* const memoryPointer) noexcept;

	private:
		/// <summary>
		///		static
		///		operator new[]()
		/// </summary>
		/// <param name="memoryArraySize"></param>
		/// <returns>void*</returns>
		static void* operator new[](std::size_t memoryArraySize);

		/// <summary>
		///		static
		///		noexcept
		///		operator delete[]()
		/// </summary>
		/// <param name="memoryArrayPointer"></param>
		/// <returns>void</returns>
		static void operator delete[](void* const memoryArrayPointer) noexcept;

		/// <summary>
		///		dynamic
		///		virtual
		///		noexcept
		///		final
		///		operator&amp;
		/// </summary>
		/// <returns>SafeContextBase*</returns>
		virtual SafeContextBase* operator&() noexcept final;

	protected:
		/// <summary>
		///		dynamic
		///		noexcept
		///		operator=
		/// </summary>
		/// <param name="other"></param>
		/// <returns>SafeContextBase&amp;</returns>
		SafeContextBase& operator=(const SafeContextBase& other) noexcept;

		/// <summary>
		///		dynamic
		///		noexcept
		///		operator=
		/// </summary>
		/// <param name="other"></param>
		/// <returns>SafeContextBase&amp;</returns>
		SafeContextBase& operator=(SafeContextBase&& other) noexcept;

	public:
		/// <summary>
		///		dynamic
		///		noexcept
		///		operator==
		/// </summary>
		/// <param name="other"></param>
		/// <returns>bool</returns>
		bool operator==(const SafeContextBase& other) const noexcept;

		/// <summary>
		///		static
		/// </summary>
		/// <returns>SafeContextBase&amp;</returns>
		static SafeContextBase& constructInstanceOnMemoryHeap();

	private:
		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		static void destructInstanceOnMemoryHeap(SafeContextBase* const instancePointer) noexcept;

		/// <summary>
		///		static
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <param name="constructionInvoker"></param>
		/// <returns>void</returns>
		static void reconstructSafely(SafeContextBase* const instancePointer,const DefaultConstructionInvoker& constructionInvoker);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <param name="constructionInvoker"></param>
		/// <returns>void</returns>
		static void helpInitializeChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality,const DefaultConstructionInvoker& constructionInvoker);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="sourceElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <param name="constructionInvoker"></param>
		/// <returns>void</returns>
		static void helpDuplicateChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::vector<SafeContextBase*>& sourceElementPointers,const std::size_t& cardinality,const CopyConstructionInvoker& constructionInvoker);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <returns>void</returns>
		static void helpDisposeChunk(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <returns>void</returns>
		static void destroyDerivedChunkOnMemoryHeap(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality);

		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <param name="typeInformation"></param>
		/// <returns>SafeContextBase*</returns>
		static SafeContextBase* upcycle(const std::type_info& typeInformation) noexcept;

		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <param name="pointer"></param>
		/// <returns>void</returns>
		static void finalize(SafeContextBase*& pointer) noexcept;

	public:
		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <returns>void</returns>
		static void enableMultithreadedMode() noexcept;

		/// <summary>
		///		static
		/// </summary>
		/// <param name="instanceReference"></param>
		/// <returns>void</returns>
		static void recycle(SafeContextBase& instanceReference);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		static void recycle(SafeContextBase* const instancePointer);

		/// <summary>
		///		static
		///		inline
		/// </summary>
		/// <typeparam name="GenericTypeOfSafeContextDerivative"></typeparam>
		/// <returns>GenericTypeOfSafeContext&amp;</returns>
		template<typename GenericTypeOfSafeContextDerivative> static inline GenericTypeOfSafeContextDerivative& repurpose()
		{
			static_assert((std::is_base_of<SafeContextBase,GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type inherited from `SafeContextBase`!");
			static_assert((std::is_default_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a default constructor!");
			static_assert((std::is_pointer<GenericTypeOfSafeContextDerivative>::value == false),"`GenericTypeOfSafeContextDerivative` can't be a pointer type!");

			GenericTypeOfSafeContextDerivative* recycledPointer = static_cast<GenericTypeOfSafeContextDerivative*>(SafeContextBase::upcycle(typeid(GenericTypeOfSafeContextDerivative)));

			if (recycledPointer != nullptr)
			{
				SafeContextBase::reconstructSafely(recycledPointer,[](SafeContextBase* const instancePointer) -> void
				{
					::new (static_cast<GenericTypeOfSafeContextDerivative*>(instancePointer)) GenericTypeOfSafeContextDerivative();
				});

				return *recycledPointer;
			}
			else
			{
				return *(::new GenericTypeOfSafeContextDerivative());
			}
		};

		/// <summary>
		///		static
		///		inline
		/// </summary>
		/// <typeparam name="GenericTypeOfSafeContextDerivative"></typeparam>
		/// <param name="chunkCardinality"></param>
		/// <returns>SafeMemoryChunk&lt;GenericTypeOfSafeContextDerivative&gt;&amp;</returns>
		template<typename GenericTypeOfSafeContextDerivative> static inline SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& createDerivedChunkOnMemoryHeap(const std::size_t& chunkCardinality)
		{
			static_assert((std::is_base_of<SafeContextBase,GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type inherited from `SafeContextBase`!");
			static_assert((std::is_default_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a default constructor!");
			static_assert((std::is_copy_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a copy constructor!");
			static_assert((std::is_move_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a move constructor!");
			static_assert((std::is_copy_assignable<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a copy assignment operator!");
			static_assert((std::is_move_assignable<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a move assignment operator!");
			static_assert((std::is_pointer<GenericTypeOfSafeContextDerivative>::value == false),"`GenericTypeOfSafeContextDerivative` can't be a pointer type!");

			return *(new SafeMemoryChunk<GenericTypeOfSafeContextDerivative>(chunkCardinality));
		};

		/// <summary>
		///		dynamic
		///		virtual
		///		noexcept
		///		final
		/// </summary>
		/// <returns>std::size_t</returns>
		virtual std::size_t getMemorySize() const noexcept final;

		/// <summary>
		///		dynamic
		///		noexcept
		/// </summary>
		/// <returns>type_info&amp;</returns>
		const std::type_info& getTypeInfo() const noexcept;

		/// <summary>
		///		dynamic
		///		noexcept
		/// </summary>
		/// <returns>size_t</returns>
		std::size_t getHashCode() const noexcept;
	};
};
