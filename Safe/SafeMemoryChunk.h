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
#include <string>
#include <type_traits>
#include <vector>

/** Inclusion(s) of project's C++ header file(s).**/
#include "SafeContextBase.h"
#include "SafeContextException.h"


/** Main code.**/

/// <summary>
///		C++ namespace: `Safe`.
/// </summary>
namespace Safe
{
	/// <summary>
	///		C++ class template: `SafeMemoryChunk`.
	/// </summary>
	/// <typeparam name="GenericTypeOfSafeContextDerivative"></typeparam>
	template<typename GenericTypeOfSafeContextDerivative> requires std::equality_comparable<GenericTypeOfSafeContextDerivative> class SafeContextBase::SafeMemoryChunk final : public SafeContextBase
	{
	private:
		GenericTypeOfSafeContextDerivative* composedBufferPointer;
		std::size_t cardinality;


		/// <summary>
		///		static
		///		inline
		/// </summary>
		/// <param name="index"></param>
		/// <param name="cardinality"></param>
		/// <returns>void</returns>
		[[noreturn]] static inline void throwOutOfBoundException(const std::size_t& index,const std::size_t& cardinality) noexcept(false)
		{
			throw SafeContextException("Argument `index` is out of bound: `" + std::to_string(index) + "` while the cardinality is `" + std::to_string(cardinality) + "`!");
		};

		/// <summary>
		///		static
		///		inline
		/// </summary>
		/// <param name="index"></param>
		/// <returns>void</returns>
		[[noreturn]] static inline void throwAccessViolationException(const std::size_t& index) noexcept(false)
		{
			throw SafeContextException("Can't access the chunk element at index " + std::to_string(index) + " because the chunk has been recycled!");
		};

	public:
		static_assert((std::is_base_of<SafeContextBase,GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type inherited from `SafeContextBase`!");
		static_assert((std::is_pointer<GenericTypeOfSafeContextDerivative>::value == false),"`GenericTypeOfSafeContextDerivative` can't be a pointer type!");
		static_assert((std::is_default_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a default constructor!");
		static_assert((std::is_copy_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a copy constructor!");
		static_assert((std::is_move_constructible<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a move constructor!");
		static_assert((std::is_copy_assignable<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a copy assignment operator!");
		static_assert((std::is_move_assignable<GenericTypeOfSafeContextDerivative>::value == true),"`GenericTypeOfSafeContextDerivative` must be a type having a move assignment operator!");

		
		/// <summary>
		///		C++ functional pointer type: `SafeMemoryChunkElementMutator`.
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		typedef void (*SafeMemoryChunkElementMutator)(GenericTypeOfSafeContextDerivative* const instancePointer);


		/// <summary>
		///		Constructor of `SafeMemoryChunk`.
		/// </summary>
		inline explicit SafeMemoryChunk() : SafeContextBase()
		{
			this->cardinality = 10;
			std::vector<SafeContextBase*> chunkBufferElementPointers = std::vector<SafeContextBase*>();
			chunkBufferElementPointers.reserve(this->cardinality);
			GenericTypeOfSafeContextDerivative* chunkPointer = static_cast<GenericTypeOfSafeContextDerivative*>(::operator new(sizeof(GenericTypeOfSafeContextDerivative) * this->cardinality));
			std::size_t i = 0;
			
			try
			{
				for (i = 0;i < this->cardinality;i++)
				{
					chunkBufferElementPointers.push_back(static_cast<SafeContextBase*>(chunkPointer + i));
				}

				this->composedBufferPointer = chunkPointer;

				SafeContextBase::helpInitializeChunk(chunkBufferElementPointers,this->cardinality,[](SafeContextBase* const instancePointer) -> void
				{
					::new (instancePointer) GenericTypeOfSafeContextDerivative();
				});
			}
			catch (...)
			{
				::operator delete(this->composedBufferPointer);
				this->composedBufferPointer = 0;
				this->cardinality = 0;
				throw;
			}
		};

		/// <summary>
		///		Constructor of `SafeMemoryChunk`.
		/// </summary>
		/// <param name="cardinality"></param>
		inline explicit SafeMemoryChunk(const std::size_t& cardinality) : SafeContextBase()
		{
			this->cardinality = cardinality;
			std::vector<SafeContextBase*> chunkBufferElementPointers = std::vector<SafeContextBase*>();
			chunkBufferElementPointers.reserve(this->cardinality);
			GenericTypeOfSafeContextDerivative* chunkPointer = static_cast<GenericTypeOfSafeContextDerivative*>(::operator new(sizeof(GenericTypeOfSafeContextDerivative) * this->cardinality));
			std::size_t i = 0;

			try
			{
				for (i = 0;i < cardinality;i++)
				{
					chunkBufferElementPointers.push_back(static_cast<SafeContextBase*>(chunkPointer + i));
				}

				this->composedBufferPointer = chunkPointer;

				SafeContextBase::helpInitializeChunk(chunkBufferElementPointers,this->cardinality,[](SafeContextBase* const instancePointer) -> void
				{
					::new (instancePointer) GenericTypeOfSafeContextDerivative();
				});
			}
			catch (...)
			{
				::operator delete(this->composedBufferPointer);
				this->composedBufferPointer = nullptr;
				this->cardinality = 0;
				throw;
			}
		};

		/// <summary>
		///		Copy constructor of `SafeMemoryChunk`.
		/// </summary>
		/// <param name="other"></param>
		inline SafeMemoryChunk(const SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& other) : SafeContextBase(static_cast<const SafeContextBase&>(other))
		{
			if (other.cardinality > 0)
			{
				this->cardinality = other.cardinality;
			}
			else
			{
				this->cardinality = 10;
			}

			std::vector<SafeContextBase*> chunkBufferElementPointers = std::vector<SafeContextBase*>();
			chunkBufferElementPointers.reserve(this->cardinality);
			GenericTypeOfSafeContextDerivative* chunkPointer = static_cast<GenericTypeOfSafeContextDerivative*>(::operator new(sizeof(GenericTypeOfSafeContextDerivative) * this->cardinality));
			std::size_t i = 0;

			if (other.composedBufferPointer == nullptr)
			{
				try
				{
					for (i = 0;i < this->cardinality;i++)
					{
						chunkBufferElementPointers.push_back(static_cast<SafeContextBase*>(chunkPointer + i));
					}

					this->composedBufferPointer = chunkPointer;

					SafeContextBase::helpInitializeChunk(chunkBufferElementPointers,this->cardinality,[](SafeContextBase* const instancePointer) -> void
					{
						::new (instancePointer) GenericTypeOfSafeContextDerivative();
					});
				}
				catch (...)
				{
					::operator delete(this->composedBufferPointer);
					this->composedBufferPointer = nullptr;
					this->cardinality = 0;
					throw;
				}
			}
			else
			{
				try
				{
					for (i = 0;i < this->cardinality;i++)
					{
						chunkBufferElementPointers.push_back(static_cast<SafeContextBase*>(chunkPointer + i));
					}

					this->composedBufferPointer = chunkPointer;

					std::vector<SafeContextBase*> otherElementPointers = std::vector<SafeContextBase*>();

					for (i = 0;i < other.cardinality;i++)
					{
						otherElementPointers.push_back(static_cast<SafeContextBase*>((other.composedBufferPointer)[i]));
					}

					SafeContextBase::helpDuplicateChunk(chunkBufferElementPointers,otherElementPointers,this->cardinality,[](SafeContextBase* const instancePointer,const SafeContextBase* const otherInstancePointer) -> void
					{
						::new (instancePointer) GenericTypeOfSafeContextDerivative(*(static_cast<const GenericTypeOfSafeContextDerivative* const>(otherInstancePointer)));
					});
				}
				catch (...)
				{
					::operator delete(this->composedBufferPointer);
					this->composedBufferPointer = nullptr;
					this->cardinality = 0;
					throw;
				}
			}
		};

		/// <summary>
		///		Move constructor of `SafeMemoryChunk`.
		/// </summary>
		/// <param name="other"></param>
		inline SafeMemoryChunk(SafeMemoryChunk<GenericTypeOfSafeContextDerivative>&& other) noexcept(false) : SafeContextBase(static_cast<SafeContextBase&&>(other))
		{
			this->cardinality = static_cast<std::size_t&&>(other.cardinality);
			other.cardinality = 0;
			this->composedBufferPointer = static_cast<GenericTypeOfSafeContextDerivative*&&>(other.composedBufferPointer);
			other.composedBufferPointer = nullptr;
		};

		/// <summary>
		///		Destructor of `SafeMemoryChunk`.
		/// </summary>
		inline virtual ~SafeMemoryChunk() noexcept(false) override
		{
			if (this->composedBufferPointer != nullptr)
			{
				std::size_t i = 0;
				std::vector<SafeContextBase*> chunkBufferElementPointers = std::vector<SafeContextBase*>();

				for (i = 0;i < this->cardinality;i++)
				{
					chunkBufferElementPointers.push_back(&((this->composedBufferPointer)[i]));
				}

				SafeContextBase::destroyDerivedChunkOnMemoryHeap(chunkBufferElementPointers,this->cardinality);
				::operator delete(this->composedBufferPointer);
				this->composedBufferPointer = nullptr;
			}

			this->cardinality = 0;
		};

		/// <summary>
		///		dynamic
		///		inline
		///		operator=
		/// </summary>
		/// <param name="other"></param>
		/// <returns>SafeMemoryChunk&lt;GenericTypeOfSafeContextDerivative&gt;&amp;</returns>
		inline SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& operator=(const SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& other) = delete;

		/// <summary>
		///		dynamic
		///		inline
		///		operator=
		/// </summary>
		/// <param name="other"></param>
		/// <returns>SafeMemoryChunk&lt;GenericTypeOfSafeContextDerivative&gt;&amp;</returns>
		inline SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& operator=(SafeMemoryChunk<GenericTypeOfSafeContextDerivative>&& other) = delete;

		/// <summary>
		///		dynamic
		///		inline
		///		operator[]
		/// </summary>
		/// <param name="index"></param>
		/// <returns>GenericTypeOfSafeContextDerivative</returns>
		inline GenericTypeOfSafeContextDerivative operator[](const std::size_t& index)
		{
			if (index >= this->cardinality)
			{
				SafeMemoryChunk<GenericTypeOfSafeContextDerivative>::throwOutOfBoundException(index,this->cardinality);
			}
			else
			{
				if (this->composedBufferPointer == nullptr)
				{
					SafeMemoryChunk<GenericTypeOfSafeContextDerivative>::throwAccessViolationException(index);
				}
				else
				{
					return (this->composedBufferPointer)[index];
				}
			}
		};

		/// <summary>
		///		dynamic
		///     inline
		///		noexcept
		/// </summary>
		/// <returns>size_t</returns>
		inline std::size_t getCardinality() const noexcept
		{
			return this->cardinality;
		};

		/// <summary>
		///		dynamic
		///     inline
		/// </summary>
		/// <returns>bool</returns>
		inline bool contains(const GenericTypeOfSafeContextDerivative& element) const
		{
			if (this->composedBufferPointer == nullptr)
			{
				return false;
			}
			else
			{
				std::size_t i = 0;

				for (i = 0;i < this->cardinality;i++)
				{
					if ((this->composedBufferPointer)[i] == element)
					{
						return true;
					}
				}

				return false;
			}
		};

		/// <summary>
		///		dynamic
		///		inline
		/// </summary>
		/// <param name="index"></param>
		/// <param name="mutator"></param>
		/// <returns>void</returns>
		inline void mutate(const std::size_t& index,const SafeMemoryChunkElementMutator& mutator)
		{
			this->mutateElementByIndex(index,mutator);
		};

		/// <summary>
		///		dynamic
		///		inline
		/// </summary>
		/// <param name="index"></param>
		/// <param name="mutator"></param>
		/// <returns>void</returns>
		inline void mutateElementByIndex(const std::size_t& index,const SafeMemoryChunkElementMutator& mutator)
		{
			if (index >= this->cardinality)
			{
				SafeMemoryChunk<GenericTypeOfSafeContextDerivative>::throwOutOfBoundException(index,this->cardinality);
			}
			else if ((this->composedBufferPointer == nullptr) || (&((this->composedBufferPointer)[index]) == nullptr))
			{
				SafeMemoryChunk<GenericTypeOfSafeContextDerivative>::throwAccessViolationException(index);
			}
			else
			{
				mutator(static_cast<GenericTypeOfSafeContextDerivative*>(&((this->composedBufferPointer)[index])));
			}
		};

		/// <summary>
		///		dynamic
		///		inline
		/// </summary>
		/// <returns>void</returns>
		inline void dispose()
		{
			if (this->composedBufferPointer != nullptr)
			{
				std::size_t i = 0;
				std::vector<SafeContextBase*> chunkBufferElementPointers = std::vector<SafeContextBase*>();

				for (i = 0;i < this->cardinality;i++)
				{
					chunkBufferElementPointers.push_back(&((this->composedBufferPointer)[i]));
				}

				SafeContextBase::helpDisposeChunk(chunkBufferElementPointers,this->cardinality);
				::operator delete(this->composedBufferPointer);
				this->composedBufferPointer = nullptr;
			}
		};
	};

	/// <summary>
	///		C++ type name alias: `SafeMemoryChunk`.
	/// </summary>
	/// <typeparam name="GenericTypeOfSafeContextDerivative"></typeparam>
	template<typename GenericTypeOfSafeContextDerivative> using SafeMemoryChunk = SafeContextBase::SafeMemoryChunk<GenericTypeOfSafeContextDerivative>;

	/// <summary>
	///		C++ type name alias: `SafeArray`.
	/// </summary>
	/// <typeparam name="GenericTypeOfSafeContextDerivative"></typeparam>
	template<typename GenericTypeOfSafeContextDerivative> using SafeArray = SafeContextBase::SafeMemoryChunk<GenericTypeOfSafeContextDerivative>;
};