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
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

/** Inclusion(s) of project's C++ header file(s).**/
#include "SafeContextBase.h"
#include "SafeContextException.h"
#include "SafeEvent.h"
#include "SafeMemoryChunk.h"
#include "SafeNamespace.h"
#include "SafeString.h"
#include "SafeTrade.h"

/** Inclusion(s) of the standard C++ namespace(s).**/
using namespace std;


/** Main code.**/

/// <summary>
///		C++ namespace: `Safe`.
/// </summary>
namespace Safe
{
	
};


/** Test cases.**/

/// <summary>
///		C++ class: `Example`.
/// </summary>
/*class Example safe
{
private:
	size_t id;

public:
	Example()
	{
		this->id = 0;
	};
	Example(const size_t& id)
	{
		this->id = id;
	};
	Example(const Example& other) noexcept
	{
		this->id = other.id;
	};
	Example(Example&& other) noexcept
	{
		this->id = move(other.id);
	};
	~Example() override = default;


	Example& operator=(const Example& other) noexcept
	{
		this->id = other.id;

		return *this;
	};

	Example& operator=(Example&& other) noexcept
	{
		this->id = move(other.id);

		return *this;
	};

	size_t getID() const noexcept
	{
		return this->id;
	};

	void setID(const size_t& id) noexcept
	{
		this->id = id;
	};

	void setID(size_t&& id) noexcept
	{
		this->id = move(id);
	};

	static Example& getRandomReference()
	{
		int randomNumber = rand() % 100;
		Example* localInstancePointer = ::new Example(randomNumber);

		return *localInstancePointer;
	};

	static Example* allocateChunkManually(const size_t& cardinality)
	{
		Example* chunkPointer = static_cast<Example*>(::operator new(sizeof(Example) * cardinality));
		size_t i = 0;

		for (i = 0;i < cardinality;i++)
		{
			//::new (&(chunkPointer[i])) Example(); //Impossible.
			//::new (chunkPointer + i) Example(); //Definitely crashes the program in the end.
		}

		return chunkPointer;
	};
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	Example* pointer = ::new Example();
	Example* anotherPointer = ::new Example();
	Example* pointerAlias = pointer;
	cout << pointer->getID() << endl;
	Safe::SafeContextBase::recycle(dynamic_cast<Safe::SafeContextBase*>(pointer));
	Example& repurposedReference = Safe::SafeContextBase::repurpose<Example>();
	cout << (repurposedReference.getTypeInfo()).name() << endl;
	cout << pointer->getID() << endl;

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	Example* chunkPointer = ::new Example();
	::delete chunkPointer; // Compilation error if compiled using Visual C++.

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	Safe::SafeMemoryChunk<Example>* chunkPointer = ::new Safe::SafeMemoryChunk<Example>(100);
	Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase*>(chunkPointer));
	Safe::SafeMemoryChunk<Example>& repurposedChunkReference = Safe::SafeContextBase::repurpose<Safe::SafeMemoryChunk<Example>>();
	cout << repurposedChunkReference.getCardinality() << endl;

	try
	{
		//Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase&>((*chunkPointer)[0])); //Impossible.
		chunkPointer->mutate(0,[](Example* const instancePointer) -> void
		{
			Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase*>(instancePointer));
		});
	}
	catch (const Safe::SafeContextException& exception)
	{
		cout << exception.getMessage() << endl;
	}

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	Safe::SafeContextBase::SafeMemoryChunk<Example>& chunkReference = Safe::SafeContextBase::createDerivedChunkOnMemoryHeap<Example>(10);
	(chunkReference[9]).setID(100);
	cout << (chunkReference[9]).getID() << endl;
	chunkReference.dispose();
	Example& reference = chunkReference[9]; //Invalid, prevented dangling reference.
	cout << reference.getID() << endl;
	const Example& constantReference = chunkReference[9]; //Valid due to lifetime extensions, prevented dangling reference.

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	const int threadCount = 8;
	const int perThread = 200;
	size_t j = 0;
	atomic<int> numberOfAllocations{ 0 };
	atomic<int> numberOfRecycles{ 0 };
	atomic<int> numberOfRepurposes{ 0 };
	vector<thread> threads = vector<thread>();
	Safe::SafeContextBase::enableMultithreadedMode(); //Crucial for multi-threading scheme.

	for (j = 0;j < threadCount;++j)
	{
		threads.emplace_back([j,perThread,&numberOfAllocations,&numberOfRecycles,&numberOfRepurposes]()
		{
			Safe::SafeMemoryChunk<Example>* chunkPointer = ::new Safe::SafeMemoryChunk<Example>(100);
			vector<Example*> instancePointers = vector<Example*>();
			instancePointers.reserve(perThread);
			size_t i = 0;

			for (i = 0;i < perThread;i++)
			{
				Example* instancePointer = ::new Example();
				numberOfAllocations++;
				instancePointers.push_back(instancePointer);
			}

			for (i = 0;i < perThread / 2;i++)
			{
				Example*& pointerReference = instancePointers[i];
				Safe::SafeContextBase* polymorphicPointer = dynamic_cast<Safe::SafeContextBase*>(pointerReference);
				Safe::SafeContextBase::recycle(polymorphicPointer);

				if (pointerReference == nullptr)
				{
					numberOfRecycles++;
				}
			}

			for (i = 0;i < perThread / 2;i++)
			{
				try
				{
					Example& repurposedReference = Safe::SafeContextBase::repurpose<Example>();
					repurposedReference.setID(j * 10000 + i);
					numberOfRepurposes++;
				}
				catch (...)
				{

				}
			}

			this_thread::sleep_for(chrono::milliseconds(10));

			for (i = perThread / 2;i < instancePointers.size();i++)
			{
				Example*& pointerReference = instancePointers[i];
				Safe::SafeContextBase* polymorphicPointer = dynamic_cast<Safe::SafeContextBase*>(pointerReference);

				if (pointerReference != nullptr)
				{
					Safe::SafeContextBase::recycle(polymorphicPointer);

					if (polymorphicPointer == nullptr)
					{
						numberOfRecycles++;
					}
				}
			}
		});
	}

	for (thread& thread : threads)
	{
		thread.join();
	}

	cout << "Number of allocations: `" << numberOfAllocations.load() << "`, recycles: `" << numberOfRecycles.load() << "` and repurposes: `" << numberOfRepurposes.load() << "`." << endl;

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	Example& reference = Example::getRandomReference();
	cout << "ID: " << reference.getID() << endl;

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	class NumericEvent final : Safe::SafeEvent
	{
	private:
		size_t number;

	public:
		explicit NumericEvent() noexcept
		{
			this->number = 0;
		};
		explicit NumericEvent(const size_t& number) noexcept
		{
			this->number = number;
		};
		NumericEvent(const NumericEvent& other) noexcept
		{
			this->number = other.number;
		};
		NumericEvent(NumericEvent&& other) noexcept
		{
			this->number = move(other.number);
		};
		~NumericEvent() = default;


		NumericEvent& operator=(const NumericEvent& other) noexcept
		{
			this->number = other.number;

			return *this;
		};

		NumericEvent& operator=(NumericEvent&& other) noexcept
		{
			this->number = move(other.number);

			return *this;
		};

		static size_t generateRandomNumber(const size_t& minimum,const size_t& maximum)
		{
			size_t randomSeeder = chrono::system_clock::now().time_since_epoch().count();
			mt19937_64 randomGenerator(randomSeeder);
			uniform_int_distribution<size_t> distribution(minimum,maximum);

			return distribution(randomGenerator);
		};

		size_t getNumber() const noexcept
		{
			return this->number;
		};

		void setNumber(const size_t& number) noexcept
		{
			this->number = number;
		};

		void incrementNumber()
		{
			this->number++;
		};

		void decrementNumber()
		{
			this->number--;
		};
	};


	NumericEvent* eventPointerA = ::new NumericEvent();
	NumericEvent* eventPointerB = ::new NumericEvent();
	size_t limit = 4;

	while (eventPointerB->getNumber() < limit)
	{
		eventPointerA->incrementNumber();

		if (eventPointerA->getNumber() % 2 == 0)
		{
			eventPointerA->decrementNumber();
		}
		else
		{
			if (NumericEvent::generateRandomNumber(1,100) % 2 == 0)
			{
				eventPointerA->incrementNumber();
				eventPointerB->setNumber(eventPointerA->getNumber());
			}
		}

		cout << "Event A: " << eventPointerA->getNumber() << endl;
		cout << "Event B: " << eventPointerB->getNumber() << endl;
	}
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif

	class ClickEvent : public Safe::SafeEvent
	{
	public:
		double x;
		double y;


		ClickEvent(const double& x,const double& y,const Safe::SafeEvent::SafeEventOccurrence& occurrence) : x(x),y(y),Safe::SafeEvent(occurrence)
		{

		};

		~ClickEvent() = default;
	};

	class MoveEvent : public Safe::SafeEvent
	{
	public:
		double x;
		double y;


		MoveEvent(double x,double y) : x(x),y(y),Safe::SafeEvent()
		{
			
		};

		~MoveEvent() = default;
	};

	class ClickableOpacity final : public Safe::SafeContextBase
	{
	public:
		Safe::SafeEventHandler<ClickEvent> onClick;
		Safe::SafeEventHandler<MoveEvent> onMove;


		ClickableOpacity() : Safe::SafeContextBase()
		{
			this->onClick = *(::new Safe::SafeEventHandler<ClickEvent>());
			this->onMove = *(::new Safe::SafeEventHandler<MoveEvent>());
		};

		virtual ~ClickableOpacity() override = default;


		void handleClick(const ClickEvent& clickEvent)
		{
			(this->onClick).handle(clickEvent);
		};
	};

	ClickableOpacity* buttonPointer = ::new ClickableOpacity();
	buttonPointer->onClick = [](const ClickEvent& event)
	{
		cout << "Mouse moved to coordinates: (" << event.x << "," << event.y << ")." << endl;
	};

	ClickEvent* eventPointer = ::new ClickEvent(0,0,[&eventPointer,&buttonPointer]() noexcept -> void
	{
		POINT mousePoint;

		if (GetCursorPos(&mousePoint) && ((eventPointer->x != mousePoint.x) || (eventPointer->y != mousePoint.y)))
		{
			eventPointer->x = mousePoint.x;
			eventPointer->y = mousePoint.y;
			buttonPointer->handleClick(*eventPointer);
		}

		if ((mousePoint.x == 0) && (mousePoint.y == 0))
		{
			eventPointer->cancel();
		}
	});
	
	eventPointer->broadcast();

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif
	Safe::SafeString<char>* stringPointer = ::new Safe::SafeString("abcdef");
	Safe::SafeString<char> string = "abcdef";
	Safe::SafeStringBuilder<char> stringBuilder = Safe::SafeStringBuilder<char>(string);
	stringBuilder.append("abcdef");
	cout << stringBuilder.getLength() << endl;

	return 0;
};*/

/*class UnsafeObject
{

};

class SafeObject final : public Safe::SafeContextBase
{

};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif
	size_t i = 0;
	size_t j = 0;
	size_t beginTick = 0;
	size_t endTick = 0;
	size_t numberOfTurns = 10000000;
	size_t elapsedTime = 0;
	size_t totalTimeUnderSafeContext = 0;
	size_t totalTimeUnderUnsafeContext = 0;
	SafeObject* safePointer = ::new SafeObject();
	Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase*>(safePointer));

	while (j < numberOfTurns)
	{
		beginTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		SafeObject& safeReference = Safe::SafeContextBase::repurpose<SafeObject>();
		Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase&>(safeReference));
		endTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		elapsedTime = endTick - beginTick;
		totalTimeUnderSafeContext += elapsedTime;
		beginTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		volatile UnsafeObject* unsafePointer = new UnsafeObject();
		delete unsafePointer;
		endTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		elapsedTime = endTick - beginTick;
		totalTimeUnderUnsafeContext += elapsedTime;
		j++;
	}

	cout << "Average time for unsafe allocation: " << ((1.0 * totalTimeUnderUnsafeContext) / 1000000000) << " second(s)." << endl;
	cout << "Average time for safe allocation: " << ((1.0 * totalTimeUnderSafeContext) / 1000000000) << " second(s)." << endl;

	return 0;
};*/

/*#ifdef WINDOWS_VISUAL_CPP
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
int main()
{
#ifdef WINDOWS_VISUAL_CPP
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_DEBUG);
#endif
	size_t i = 0;
	size_t j = 0;
	size_t beginTick = 0;
	size_t endTick = 0;
	size_t numberOfTurns = 100000;
	size_t elapsedTime = 0;
	size_t totalTimeUnderSafeContext = 0;
	size_t totalTimeUnderUnsafeContext = 0;
	Safe::SafeVector<SafeObject>* safeVectorPointer = ::new Safe::SafeVector<SafeObject>();
	Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase*>(safeVectorPointer));
	UnsafeObject unsafeObject = UnsafeObject();

	while (j < numberOfTurns)
	{
		beginTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		Safe::SafeVector<SafeObject>& safeVectorReference = Safe::SafeContextBase::repurpose<Safe::SafeVector<SafeObject>>();
		Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase&>(safeVectorReference));
		endTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		elapsedTime = endTick - beginTick;
		totalTimeUnderSafeContext += elapsedTime;
		beginTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		volatile vector<UnsafeObject>* unsafeVectorPointer = new vector<UnsafeObject>(10,unsafeObject);
		delete unsafeVectorPointer;
		endTick = chrono::high_resolution_clock::now().time_since_epoch().count();
		elapsedTime = endTick - beginTick;
		totalTimeUnderUnsafeContext += elapsedTime;
		j++;
	}

	cout << "Average time for unsafe allocation: " << ((1.0 * totalTimeUnderUnsafeContext) / 1000000000) << " second(s)." << endl;
	cout << "Average time for safe allocation: " << ((1.0 * totalTimeUnderSafeContext) / 1000000000) << " second(s)." << endl;

	return 0;
};*/