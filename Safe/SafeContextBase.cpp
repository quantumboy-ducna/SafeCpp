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

/** Inclusion(s) of C++ standard library header file(s).**/
#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <deque>
#include <mutex>
#include <string>
#include <typeinfo>
#include <vector>

/** Inclusion(s) of project's C++ header file(s).**/
#include "SafeContextBase.h"
#include "SafeContextException.h"

/** Inclusion(s) of the standard C++ namespace(s).**/
using namespace std;


/** Main code.**/

namespace Safe
{
	/// <summary>
	///		C++ class: `SafeMemoryManager`.
	/// </summary>
	class SafeContextBase::SafeMemoryManager final
	{
	private:
		static atomic<bool> Classification;
		static recursive_mutex ClassificationGuard;
		static mutex ShutdownGuard;


		deque<SafeContextBase*> trackedInstancePointerQueue;
		vector<SafeContextBase*> recycledInstancePointers;
		recursive_mutex memoryTrackerSynchronizationGuard;
		mutex memoryRecyclerSynchronizationGuard;


		/// <summary>
		///		dynamic
		///		noexcept
		/// </summary>
		/// <returns>void</returns>
		void shutdown() noexcept;

	public:
		/// <summary>
		///		C++ class: `ScopedClassification`.
		/// </summary>
		class ScopedClassification final
		{
		private:
			unique_lock<recursive_mutex> locker;
			bool previousState;

		public:
			/// <summary>
			///		Constructor of `ScopedClassification`.
			/// </summary>
			/// <param name="value"></param>
			explicit ScopedClassification(const bool& state);

			/// <summary>
			///		Copy constructor of `ScopedClassification`.
			/// </summary>
			ScopedClassification(const ScopedClassification&) = delete;

			/// <summary>
			///		Move constructor of `ScopedClassification`.
			/// </summary>
			ScopedClassification(ScopedClassification&&) = delete;

			/// <summary>
			///		Destructor of `ScopedClassification`.
			/// </summary>
			~ScopedClassification() noexcept;

			/// <summary>
			///		dynamic
			///		operator=
			/// </summary>
			/// <returns>ScopedClassification&amp;</returns>
			ScopedClassification& operator=(const ScopedClassification&) = delete;

			/// <summary>
			///		dynamic
			///		operator=
			/// </summary>
			/// <returns>ScopedClassification&amp;</returns>
			ScopedClassification& operator=(ScopedClassification&&) = delete;
		};

		static atomic<bool> Multithreading;
		static SafeMemoryManager& StaticMemoryManagerReference;


		/// <summary>
		///		Constructor of `SafeMemoryManager`.
		/// </summary>
		explicit SafeMemoryManager() noexcept;

		/// <summary>
		///		Copy constructor of `SafeMemoryManager`.
		/// </summary>
		inline SafeMemoryManager(const SafeMemoryManager&) = delete;

		/// <summary>
		///		Move constructor of `SafeMemoryManager`.
		/// </summary>
		inline SafeMemoryManager(SafeMemoryManager&&) noexcept = delete;

		/// <summary>
		///		Destructor of `SafeMemoryManager`.
		/// </summary>
		~SafeMemoryManager() noexcept;

		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <returns>bool</returns>
		static bool getClassification() noexcept;

		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <param name="classification"></param>
		/// <returns>void</returns>
		static void setClassification(const bool& classification) noexcept;

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <param name="constructionInvoker"></param>
		/// <returns>void</returns>
		static void helpInitializeChunk(vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality,const DefaultConstructionInvoker& constructionInvoker);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="sourceElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <param name="constructionInvoker"></param>
		/// <returns>void</returns>
		static void helpDuplicateChunk(vector<SafeContextBase*>& chunkBufferElementPointers,const vector<SafeContextBase*>& sourceElementPointers,const size_t& cardinality,const CopyConstructionInvoker& constructionInvoker);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <returns>void</returns>
		static void helpDestroyChunk(const vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="chunkBufferElementPointers"></param>
		/// <param name="cardinality"></param>
		/// <returns>void</returns>
		static void helpDisposeChunk(const vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality);

		/// <summary>
		///		static
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <param name="constructionInvoker"></param>
		/// <returns>void</returns>
		static void reconstructSafely(SafeContextBase* const instancePointer,const DefaultConstructionInvoker& constructionInvoker);

		/// <summary>
		///		static
		///		noexcept
		/// </summary>
		/// <returns>SafeContextBase::SafeMemoryManager&amp;</returns>
		static SafeContextBase::SafeMemoryManager& initializeStaticMemoryManager() noexcept;

		/// <summary>
		///		dynamic
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		void emplaceInstance(SafeContextBase* const instancePointer);

		/// <summary>
		///		dynamic
		///		noexcept
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		void displaceInstance(SafeContextBase* const instancePointer) noexcept;

		/// <summary>
		///		dynamic
		/// </summary>
		/// <param name="instanceReference"></param>
		/// <returns>void</returns>
		void recycle(SafeContextBase& instanceReference);

		/// <summary>
		///		dynamic
		/// </summary>
		/// <param name="instancePointer"></param>
		/// <returns>void</returns>
		/// <exception cref="SafeContextException"/>
		void recycle(SafeContextBase* const instancePointer);

		/// <summary>
		///		dynamic
		///		noexcept
		/// </summary>
		/// <param name="typeInformation"></param>
		/// <returns>SafeContextBase*</returns>
		SafeContextBase* upcycle(const type_info& typeInformation) noexcept;

		/// <summary>
		///		dynamic
		/// </summary>
		/// <param name="pointerInDangerZone"></param>
		/// <returns>void</returns>
		void forceShutdown(const SafeContextBase* pointerInDangerZone);
	};


	SafeContextBase::SafeMemoryManager::ScopedClassification::ScopedClassification(const bool& state) : locker(ClassificationGuard,std::defer_lock),previousState(false)
	{
		if (Multithreading.load() == true)
		{
			(this->locker).lock();
		}

		this->previousState = Classification.exchange(state);
	};
	SafeContextBase::SafeMemoryManager::ScopedClassification::~ScopedClassification() noexcept
	{
		Classification.store(previousState);
	};

	SafeContextBase::SafeMemoryManager::SafeMemoryManager() noexcept : memoryTrackerSynchronizationGuard(),memoryRecyclerSynchronizationGuard()
	{
		this->trackedInstancePointerQueue = deque<SafeContextBase*>();
		this->recycledInstancePointers = vector<SafeContextBase*>();
	};
	SafeContextBase::SafeMemoryManager::~SafeMemoryManager() noexcept
	{
		this->shutdown();
	};

	SafeContextBase::SafeContextBase()
	{
		this->life = true;

		if (SafeContextBase::SafeMemoryManager::getClassification() == true)
		{
			SafeMemoryManager::StaticMemoryManagerReference.emplaceInstance(this);
		}
	};
	SafeContextBase::SafeContextBase(const bool& classification)
	{
		this->life = true;

		if (classification == true)
		{
			SafeMemoryManager::StaticMemoryManagerReference.emplaceInstance(this);
		}
	};
	SafeContextBase::SafeContextBase(const SafeContextBase& other) noexcept(false)
	{
		this->life = true;

		if (SafeContextBase::SafeMemoryManager::getClassification() == true)
		{
			SafeMemoryManager::StaticMemoryManagerReference.emplaceInstance(this);
		}
	};
	SafeContextBase::SafeContextBase(SafeContextBase&& other) noexcept(false)
	{
		this->life = true;

		if (SafeContextBase::SafeMemoryManager::getClassification() == true)
		{
			SafeMemoryManager::StaticMemoryManagerReference.emplaceInstance(this);
		}
	};
	SafeContextBase::~SafeContextBase() noexcept(false)
	{
		if (SafeContextBase::SafeMemoryManager::getClassification() == true)
		{
			SafeMemoryManager::StaticMemoryManagerReference.displaceInstance(this);

			if (this->life == false)
			{
				SafeMemoryManager::StaticMemoryManagerReference.forceShutdown(this);

				throw SafeContextException("The destructor of `SafeContextBase` is invoked manually on a polymorphic instance of type `SafeContextBase` which isn't managed or tracked!");
			}
			else
			{
				this->life = false;
			}
		}
	};


	atomic<bool> SafeContextBase::SafeMemoryManager::Classification{true};
	recursive_mutex SafeContextBase::SafeMemoryManager::ClassificationGuard = recursive_mutex();
	mutex SafeContextBase::SafeMemoryManager::ShutdownGuard = mutex();
	atomic<bool> SafeContextBase::SafeMemoryManager::Multithreading{false};
	SafeContextBase::SafeMemoryManager& SafeContextBase::SafeMemoryManager::StaticMemoryManagerReference = SafeContextBase::SafeMemoryManager::initializeStaticMemoryManager();


	void SafeContextBase::SafeMemoryManager::shutdown() noexcept
	{
		size_t i = 0;
		
		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<mutex> shutdownLock(ShutdownGuard);

			while (true)
			{
				SafeContextBase* currentInstancePointer = nullptr;

				if (currentInstancePointer == nullptr)
				{
					lock_guard<recursive_mutex> trackerLock(this->memoryTrackerSynchronizationGuard);

					if ((this->trackedInstancePointerQueue).empty() == true)
					{
						break;
					}
					else
					{
						currentInstancePointer = (this->trackedInstancePointerQueue).front();
						(this->trackedInstancePointerQueue).pop_front();
					}
				}

				::delete currentInstancePointer;
			}
		}
		else
		{
			while (true)
			{
				SafeContextBase* currentInstancePointer = nullptr;

				if (currentInstancePointer == nullptr)
				{
					if ((this->trackedInstancePointerQueue).empty() == true)
					{
						break;
					}
					else
					{
						currentInstancePointer = (this->trackedInstancePointerQueue).front();
						(this->trackedInstancePointerQueue).pop_front();
					}
				}

				::delete currentInstancePointer;
			}
		}
	};

	bool SafeContextBase::SafeMemoryManager::getClassification() noexcept
	{
		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<recursive_mutex> classificationLock(ClassificationGuard);

			return Classification.load();
		}
		else
		{
			return Classification.load();
		}
	};

	void SafeContextBase::SafeMemoryManager::setClassification(const bool& classification) noexcept
	{
		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<recursive_mutex> classificationLock(ClassificationGuard);

			SafeMemoryManager::Classification.store(classification);
		}
		else
		{
			SafeMemoryManager::Classification.store(classification);
		}
	};

	void SafeContextBase::SafeMemoryManager::helpInitializeChunk(vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality,const DefaultConstructionInvoker& constructionInvoker)
	{
		SafeContextBase::SafeMemoryManager::ScopedClassification classifier(false);
		size_t i = 0;
		size_t constructionCardinality = 0;

		try
		{
			for (i = 0;i < cardinality;i++)
			{
				constructionInvoker(chunkBufferElementPointers[i]);
				constructionCardinality++;
			}
		}
		catch (...)
		{
			for (i = 0;i < constructionCardinality;i++)
			{
				(chunkBufferElementPointers[i])->~SafeContextBase();
			}

			throw;
		}
	};

	void SafeContextBase::SafeMemoryManager::helpDuplicateChunk(vector<SafeContextBase*>& chunkBufferElementPointers,const vector<SafeContextBase*>& sourceElementPointers,const size_t& cardinality,const CopyConstructionInvoker& constructionInvoker)
	{
		SafeContextBase::SafeMemoryManager::ScopedClassification classifier(false);
		size_t i = 0;
		size_t constructionCardinality = 0;

		try
		{
			for (i = 0;i < cardinality;i++)
			{
				constructionInvoker(chunkBufferElementPointers[i],sourceElementPointers[i]);
				constructionCardinality++;
			}
		}
		catch (...)
		{
			for (i = 0;i < constructionCardinality;i++)
			{
				(chunkBufferElementPointers[i])->~SafeContextBase();
			}

			throw;
		}
	};

	void SafeContextBase::SafeMemoryManager::helpDestroyChunk(const vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality)
	{
		SafeContextBase::SafeMemoryManager::ScopedClassification classifier(false);
		size_t i = 0;

		for (i = 0;i < cardinality;i++)
		{
			(chunkBufferElementPointers[i])->~SafeContextBase();
		}
	};

	void SafeContextBase::SafeMemoryManager::helpDisposeChunk(const vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality)
	{
		SafeContextBase::SafeMemoryManager::ScopedClassification classifier(false);
		size_t i = 0;

		for (i = 0;i < cardinality;i++)
		{
			(chunkBufferElementPointers[i])->~SafeContextBase();
		}
	};

	void SafeContextBase::SafeMemoryManager::reconstructSafely(SafeContextBase* const instancePointer,const DefaultConstructionInvoker& constructionInvoker)
	{
		SafeContextBase::SafeMemoryManager::ScopedClassification classifier(false);
		static_cast<SafeContextBase*>(instancePointer)->~SafeContextBase();
		constructionInvoker(instancePointer);
	};

	SafeContextBase::SafeMemoryManager& SafeContextBase::SafeMemoryManager::initializeStaticMemoryManager() noexcept
	{
		static SafeMemoryManager instance = SafeMemoryManager();

		return instance;
	};

	void SafeContextBase::SafeMemoryManager::emplaceInstance(SafeContextBase* const instancePointer)
	{
		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<recursive_mutex> trackerLock(this->memoryTrackerSynchronizationGuard);
			(this->trackedInstancePointerQueue).push_back(instancePointer);
		}
		else
		{
			(this->trackedInstancePointerQueue).push_back(instancePointer);
		}
	};

	void SafeContextBase::SafeMemoryManager::displaceInstance(SafeContextBase* const instancePointer) noexcept
	{
		size_t i = 0;

		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<recursive_mutex> trackerLock(this->memoryTrackerSynchronizationGuard);
			lock_guard<mutex> recyclerLock(this->memoryRecyclerSynchronizationGuard);
			size_t numberOfTrackedInstances = (this->trackedInstancePointerQueue).size();

			for (i = 0;i < numberOfTrackedInstances;i++)
			{
				if ((this->trackedInstancePointerQueue)[i] == instancePointer)
				{
					(this->trackedInstancePointerQueue).erase((this->trackedInstancePointerQueue).begin() + i);
					break;
				}
			}

			i = 0;

			while (i < (this->recycledInstancePointers).size())
			{
				if ((this->recycledInstancePointers)[i] == instancePointer)
				{
					(this->recycledInstancePointers).erase((this->recycledInstancePointers).begin() + i);
				}
				else
				{
					i++;
				}
			}
		}
		else
		{
			size_t numberOfTrackedInstances = (this->trackedInstancePointerQueue).size();

			for (i = 0;i < numberOfTrackedInstances;i++)
			{
				if ((this->trackedInstancePointerQueue)[i] == instancePointer)
				{
					(this->trackedInstancePointerQueue).erase((this->trackedInstancePointerQueue).begin() + i);
					break;
				}
			}

			i = 0;

			while (i < (this->recycledInstancePointers).size())
			{
				if ((this->recycledInstancePointers)[i] == instancePointer)
				{
					(this->recycledInstancePointers).erase((this->recycledInstancePointers).begin() + i);
				}
				else
				{
					i++;
				}
			}
		}
	};

	void SafeContextBase::SafeMemoryManager::recycle(SafeContextBase& instanceReference)
	{
		this->recycle(&instanceReference);
	};

	void SafeContextBase::SafeMemoryManager::recycle(SafeContextBase* const instancePointer)
	{
		bool existence = false;
		size_t i = 0;
		size_t j = 0;

		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<recursive_mutex> trackerLock(this->memoryTrackerSynchronizationGuard);
			lock_guard<mutex> recyclerLock(this->memoryRecyclerSynchronizationGuard);
			size_t numberOfTrackedInstances = (this->trackedInstancePointerQueue).size();

			for (i = 0;i < numberOfTrackedInstances;i++)
			{
				if (instancePointer == (this->trackedInstancePointerQueue)[i])
				{
					existence = true;

					if (find((this->recycledInstancePointers).begin(),(this->recycledInstancePointers).end(),instancePointer) == (this->recycledInstancePointers).end())
					{
						(this->recycledInstancePointers).push_back(instancePointer);
					}
					else
					{
						throw SafeContextException("The argument `instancePointer` is a pointer to an already recycled instance: `" + to_string(reinterpret_cast<uintptr_t>(instancePointer)) + "`!");
					}

					break;
				}
			}

			if (existence == false)
			{
				throw SafeContextException("The argument `instancePointer` is a pointer that doesn't hold a valid memory address of an managed instance: `" + to_string(reinterpret_cast<uintptr_t>(instancePointer)) + "`! It could be a memory address of an element of the memory chunk type `SafeMemoryChunk`.");
			}
		}
		else
		{
			size_t numberOfTrackedInstances = (this->trackedInstancePointerQueue).size();

			for (i = 0;i < numberOfTrackedInstances;i++)
			{
				if (instancePointer == (this->trackedInstancePointerQueue)[i])
				{
					existence = true;

					if (find((this->recycledInstancePointers).begin(),(this->recycledInstancePointers).end(),instancePointer) == (this->recycledInstancePointers).end())
					{
						(this->recycledInstancePointers).push_back(instancePointer);
					}
					else
					{
						throw SafeContextException("The argument `instancePointer` is a pointer to an already recycled instance: `" + to_string(reinterpret_cast<uintptr_t>(instancePointer)) + "`!");
					}

					break;
				}
			}

			if (existence == false)
			{
				throw SafeContextException("The argument `instancePointer` is a pointer that doesn't hold a valid memory address of an managed instance: `" + to_string(reinterpret_cast<uintptr_t>(instancePointer)) + "`! It could be a memory address of an element of the memory chunk type `SafeMemoryChunk`.");
			}
		}
	};

	SafeContextBase* SafeContextBase::SafeMemoryManager::upcycle(const type_info& typeInformation) noexcept
	{
		size_t i = 0;
		SafeContextBase* result = nullptr;

		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<mutex> recyclerLock(this->memoryRecyclerSynchronizationGuard);
			size_t numberOfRecycledPointers = (this->recycledInstancePointers).size();

			for (i = 0;i < numberOfRecycledPointers;i++)
			{
				if (((this->recycledInstancePointers)[i])->getTypeInfo() == typeInformation)
				{
					result = (this->recycledInstancePointers)[i];
					(this->recycledInstancePointers).erase((this->recycledInstancePointers).begin() + i);
					break;
				}
			}
		}
		else
		{
			size_t numberOfRecycledPointers = (this->recycledInstancePointers).size();

			for (i = 0;i < numberOfRecycledPointers;i++)
			{
				if (((this->recycledInstancePointers)[i])->getTypeInfo() == typeInformation)
				{
					result = (this->recycledInstancePointers)[i];
					(this->recycledInstancePointers).erase((this->recycledInstancePointers).begin() + i);
					break;
				}
			}
		}

		return result;
	};

	void SafeContextBase::SafeMemoryManager::forceShutdown(const SafeContextBase* pointerInDangerZone)
	{
		size_t i = 0;
		
		if (SafeContextBase::SafeMemoryManager::Multithreading == true)
		{
			lock_guard<mutex> shutdownLock(ShutdownGuard);

			while (true)
			{
				SafeContextBase* currentInstancePointer = nullptr;

				if (currentInstancePointer == nullptr)
				{
					lock_guard<recursive_mutex> trackerLock(this->memoryTrackerSynchronizationGuard);

					if ((this->trackedInstancePointerQueue).empty() == true)
					{
						break;
					}
					else
					{
						currentInstancePointer = (this->trackedInstancePointerQueue).front();

						if (currentInstancePointer != pointerInDangerZone)
						{
							(this->trackedInstancePointerQueue).pop_front();
						}
					}
				}

				::delete currentInstancePointer;
			}
		}
		else
		{
			while (true)
			{
				SafeContextBase* currentInstancePointer = nullptr;

				if (currentInstancePointer == nullptr)
				{
					if ((this->trackedInstancePointerQueue).empty() == true)
					{
						break;
					}
					else
					{
						currentInstancePointer = (this->trackedInstancePointerQueue).front();

						if (currentInstancePointer != pointerInDangerZone)
						{
							(this->trackedInstancePointerQueue).pop_front();
						}
					}
				}

				::delete currentInstancePointer;
			}
		}
	};

	void* SafeContextBase::operator new(size_t memorySize)
	{
		return ::operator new(memorySize);
	};

	void* SafeContextBase::operator new[](size_t memoryArraySize)
	{
		return static_cast<SafeContextBase*>(::operator new[](memoryArraySize));
	};

	void SafeContextBase::operator delete(void* const memoryPointer) noexcept
	{
		::operator delete(memoryPointer);
	};

	void SafeContextBase::operator delete[](void* const memoryArrayPointer) noexcept
	{
		::operator delete[](memoryArrayPointer);
	};

	SafeContextBase* SafeContextBase::operator&() noexcept
	{
		return this;
	};

	SafeContextBase& SafeContextBase::operator=(const SafeContextBase& other) noexcept
	{
		return *this;
	};

	SafeContextBase& SafeContextBase::operator=(SafeContextBase&& other) noexcept
	{
		return *this;
	};

	bool SafeContextBase::operator==(const SafeContextBase& other) const noexcept
	{
		return (this == &other);
	};

	SafeContextBase& SafeContextBase::constructInstanceOnMemoryHeap()
	{
		return *(::new SafeContextBase());
	};

	void SafeContextBase::destructInstanceOnMemoryHeap(SafeContextBase* const instancePointer) noexcept
	{
		SafeContextBase::SafeMemoryManager::ScopedClassification classifier(false);
		SafeMemoryManager::StaticMemoryManagerReference.displaceInstance(instancePointer);
		::delete instancePointer;
	};

	void SafeContextBase::reconstructSafely(SafeContextBase* const instancePointer,const DefaultConstructionInvoker& constructionInvoker)
	{
		SafeMemoryManager::reconstructSafely(instancePointer,constructionInvoker);
	};

	void SafeContextBase::helpInitializeChunk(vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality,const DefaultConstructionInvoker& constructionInvoker)
	{
		SafeMemoryManager::helpInitializeChunk(chunkBufferElementPointers,cardinality,constructionInvoker);
	};

	void SafeContextBase::helpDuplicateChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::vector<SafeContextBase*>& sourceElementPointers,const std::size_t& cardinality,const CopyConstructionInvoker& constructionInvoker)
	{
		SafeMemoryManager::helpDuplicateChunk(chunkBufferElementPointers,sourceElementPointers,cardinality,constructionInvoker);
	};

	void SafeContextBase::destroyDerivedChunkOnMemoryHeap(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality)
	{
		SafeMemoryManager::helpDestroyChunk(chunkBufferElementPointers,cardinality);
	};

	void SafeContextBase::helpDisposeChunk(const vector<SafeContextBase*>& chunkBufferElementPointers,const size_t& cardinality)
	{
		SafeMemoryManager::helpDisposeChunk(chunkBufferElementPointers,cardinality);
	};

	SafeContextBase* SafeContextBase::upcycle(const type_info& typeInformation) noexcept
	{
		return SafeMemoryManager::StaticMemoryManagerReference.upcycle(typeInformation);
	};

	void SafeContextBase::finalize(SafeContextBase*& pointer) noexcept
	{
		pointer = nullptr;
	};

	void SafeContextBase::enableMultithreadedMode() noexcept
	{
		SafeContextBase::SafeMemoryManager::Multithreading = true;
	};

	void SafeContextBase::recycle(SafeContextBase& instanceReference)
	{
		SafeMemoryManager::StaticMemoryManagerReference.recycle(instanceReference);
	};

	void SafeContextBase::recycle(SafeContextBase* const instancePointer)
	{
		SafeMemoryManager::StaticMemoryManagerReference.recycle(instancePointer);
	};

	size_t SafeContextBase::getMemorySize() const noexcept
	{
		return sizeof(*this);
	};

	const type_info& SafeContextBase::getTypeInfo() const noexcept
	{
		return typeid(*this);
	};

	size_t SafeContextBase::getHashCode() const noexcept
	{
		return typeid(*this).hash_code();
	};
};