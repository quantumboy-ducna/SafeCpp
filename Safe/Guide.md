## Guide

### Introduction
**Safe** is a library that implements a safe context for C++. It is an effort to provide traits for types that reduce memory leaks, double deletions and other memory errors.


### Quick Start

- Requirements: C\+\+17 or later and a compatible C++ compiler.
- Choose the library type to link to your project(s).
	- If using the dynamic library version of `Safe` on Windows, only define the macro `CPP_SAFE_DYNAMICALLY_LINKED_LIBRARY_IMPORT` in consumer projects.
	- If using the static library version of `Safe` on Windows, only define the macro `CPP_SAFE_STATICALLY_LINKED_LIBRARY_IMPORT` in consumer projects.
- Select the correct runtime library version for your project(s).
	- If using the dynamic library version of `Safe` on Windows, select `/MDd` for Debug mode or `/MD` for Release mode.
	- If using the static library version of `Safe` on Windows, select `/MTd` for Debug mode or `/MT` for Release mode.

	
### Basic Usage

It's straightforward to use the `Safe` library. Below are short examples that demonstrate the common APIs. These examples use a class called `Example` that inherits from `Safe::SafeContextBase`.

```c++
#include <cstdlib>
#include <utility>

class Example : public Safe::SafeContextBase //or `class Example safe`
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
		this->id = std::move(other.id);
	};
	~Example() override = default;


	Example& operator=(const Example& other) noexcept
	{
		this->id = other.id;

		return *this;
	};

	Example& operator=(Example&& other) noexcept
	{
		this->id = std::move(other.id);

		return *this;
	};

	static Example& getRandomReference()
	{
		int randomNumber = rand() % 100;
		Example* localInstancePointer = ::new Example(randomNumber);

		return *localInstancePointer;
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
		this->id = std::move(id);
	};
};
```

1) Allocate and construct an instance of `Example`

	The below example shows how to allocate an instance of `Example` on memory heap.

```c++
int main()
{
	Example* pointer = ::new Example();
	// Do not call `::delete` on managed instances. Use `Safe::SafeContextBase::recycle(...)` or the provided deallocation helpers when you need to return memory to the system. Calling `delete` on managed instances will at least cause program termination or other compilation errors with some toolchains such as Visual C++ compiler.
};
```


2) Recycle and repurpose an instance of `Example`

	The below example shows how to properly recycle a polymorphic instance of `SafeContextBase`.

```c++
int main()
{
	Example* pointer = ::new Example();
	Example* anotherPointer = ::new Example();
	Example* pointerAlias = pointer;
	Safe::SafeContextBase::recycle(static_cast<Safe::SafeContextBase*>(pointer)); // `pointer` is no longer meaningful after recycling.
	Example& repurposedReference = Safe::SafeContextBase::repurpose<Example>();

	return 0;
};
```

3) Create and dispose a derived chunk

	The below example shows that the memory chunk is still accessible even after being disposed.

```c++
int main()
{
	Safe::SafeContextBase::SafeMemoryChunk<Example>& chunkReference = Safe::SafeContextBase::createDerivedChunkOnMemoryHeap<Example>(10);
	(chunkReference[9]).setID(100);
	cout << (chunkReference[9]).getID() << endl;
	chunkReference.dispose();
	Example& reference = chunkReference[9]; //Invalid, prevented dangling reference.
	cout << reference.getID() << endl;
	const Example& constantReference = chunkReference[9]; //Valid due to lifetime extensions, prevented dangling reference.

	return 0;
};
```

4) Access a random instance referred by a reference in another outer scope

	The below example shows that `reference` is always valid as the instance it refers to is allocated on memory heap.

```c++
int main()
{
	Example& reference = Example::getRandomReference();
	cout << "ID: " << reference.getID() << endl; // Valid
	Safe::SafeContextBase::recycle(reference);

	return 0;
};
```

5) Use an event type with an event handler type

	The below example illustrates a way to declare an event called `MoveEvent` and use another type called `Button` to handle the event using Windows API.

```c++
#include <Windows.h>

using namespace std;

class MoveEvent : public Safe::SafeEvent
{
public:
	double x;
	double y;


	MoveEvent(const double& x,const double& y,const Safe::SafeEvent::SafeEventOccurrence& occurrence) : x(x),y(y),Safe::SafeEvent(occurrence)
	{

	};

	~MoveEvent() = default;
};

class Button final : public Safe::SafeContextBase
{
public:
	Safe::SafeEventHandler<MoveEvent> onMove;


	Button() : Safe::SafeContextBase()
	{
		this->onMove = *(::new Safe::SafeEventHandler<MoveEvent>());
	};

	virtual ~Button() override = default;


	void handleClick(const MoveEvent& moveEvent)
	{
		(this->onMove).handle(moveEvent);
	};
};

int main()
{
	Button* buttonPointer = ::new Button();
	buttonPointer->onMove = [](const MoveEvent& event)
	{
		cout << "Mouse moved to coordinates: (" << event.x << "," << event.y << ")." << endl;
	};

	MoveEvent* eventPointer = ::new MoveEvent(0,0,[&eventPointer,&buttonPointer]() noexcept -> void
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
};
```


### Notice

- Although the project is referred as "runtime library", it can catch several memory errors at compile-time using its own special mechanisms and behaviors.
- There are several limitations of `Safe` that originate from language constraints of C++. These limitations are considered "unsafe" in a similar sense to unsafe contexts in languages like C# or Rust.
- The first one is, if a type that inherits from `SafeContextBase` and also manages resource types that don't inherit `SafeContextBase`, then those resources are deemed "unmanaged" and must be managed manually (that is, in the type's destructor), just like in C#'s destructors and its `IDisposable` interface.
- Pointer arithmetic is considered unsafe.
- Trying to use multi-threaded schemes without first enabling multi-threaded mode by `enableMultithreadedMode` is an undefined behavior.
- If a polymorphic instance of `SafeContextBase` is allocated on memory stack, unlike the default allocation on memory heap, it shouldn't be used with reference(s) and pointer(s) as the reference(s) and the pointer(s) can potentially exist longer than the instance, and this leads to undefined behaviors. Pointer(s) and especially reference(s) in C++ behave as alias and it doesn't have any mechanism to stop them from referencing deallocated instances on memory stack. Always use reference(s) and pointer(s) with instance(s) allocated on heap-based memory, and use instance(s) allocated on stack-based memory as value type(s).
- Constructors for types derived from `SafeContextBase` must be invoked using the global `new` (i.e. `::new`), not placement `new`. Manual arena allocation(s) with placement new will lead to undefined behavior(s) and apparently crash because the library manages and frees underlying buffers differently.

```c++
Example* chunkPointer = static_cast<Example*>(::operator new(sizeof(Example) * cardinality));
size_t i = 0;

for (i = 0;i < cardinality;i++)
{
	::new (chunkPointer + i) Example(); // Likely to cause a crash with this library.
}
```

- C++ doesn't provide a way to detect whether an instance was constructed with standard `new` or placement `new`, nor to determine whether a pointer still refers to valid storage. The library therefore attempts to extend object lifetimes; memory is reclaimed by recycling and repurposing instances. This approach can increase memory usage, so use it carefully.
- Another issue is casting. C++ cast operators such as `static_cast`, `const_cast` and especially `reinterpret_cast` can produce very unsafe conversions. For example:

```c++
Example* pointer = ::new Example();
void* pointer = reinterpret_cast<void*>(pointer);
::delete pointer; //This is very dangerous
```

- Finally, out-of-memory conditions must be handled by the application. Large managed instances should be recycled and repurposed when they are no longer needed; the same applies to chunks. Unlike Rust where the ownership system exists, the valid references or pointers to recycled instances will potentially be meaningless and can unexpectedly modify the repurposed instances.

- All of these issues are considered "unsafe". Contexts that only use managed types are the intended safe usage. I will continue working to address these limitations to enhance the safety of the safe context.


### Thread-safety

The library is implemented to be thread-safe if enabling multi-threaded mode.


### Contribution

	Duc Anh Nguyen (Myself)


### Legal & Licensing Information

Required Notice: Copyright©2026, Nguyễn Anh Đức (workofduc@gmail.com)

`SafeCpp` is dual-licensed to accommodate both open-source development and proprietary commercial use:

* **Open-Source Track (GPLv3):** This project is licensed under the **GNU General Public License v3**. It is 100% free and open for personal study, academic research, and open-source projects. In accordance with the GPLv3, any software that integrates this library under this track must also have its full source code made public.
* **Commercial Track:** For commercial entities wishing to embed `SafeCpp` into proprietary, closed-source software, a separate commercial license is required. This grants you the legal right to use the library without being bound by the GPLv3 copyleft requirements.

For commercial licensing inquiries, pricing, or to obtain a proprietary license agreement, please contact: workofduc@gmail.com.