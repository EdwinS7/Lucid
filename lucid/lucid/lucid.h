#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

#include <Windows.h>
#include <iostream>

template <typename T>
class singleton
{
protected:
	singleton() { }
	~singleton() { }

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
public:
	static T& get_instance() {
		static T inst { };

		return inst;
	}
};

/* lucid engine backend */
#include "window/wnd.h"
#include "graphics/graphics.h"

namespace lucid_engine {

}