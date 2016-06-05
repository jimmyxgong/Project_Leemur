#pragma once


#include "Commons.h"

#include <atomic>
#include <chrono>
#include <future>
#include <pplawait.h>
#include <experimental/generator>

using namespace std::experimental;

struct Resumable {
	struct promise_type {
		Resumable get_return_object() {
			return Resumable(coroutine_handle<promise_type>::from_promise(this));
		}

		auto initial_suspend() {
			return suspend_never{};
		}

		auto final_suspend() {
			return suspend_never{};
		}
	};

	coroutine_handle<promise_type> coroutine = nullptr;
	explicit Resumable(coroutine_handle<promise_type> routine):
		coroutine(routine) {}

	Resumable() = default;
	Resumable(Resumable const &) = delete;
	Resumable& operator=(Resumable const &) = delete;
	Resumable(Resumable && rhs) :
		coroutine(rhs.coroutine)
	{
		rhs.coroutine = nullptr;
	}
	Resumable& operator=(Resumable && rhs) {
		if (&rhs != this) {
			coroutine = rhs.coroutine;
			rhs.coroutine = nullptr;
		}

		return *this;
	}
	//Resumable()

	~Resumable() {
		/*if (coroutine) {
			coroutine.destroy();
		}*/
	}


	void resume() const {
		coroutine.resume();
	}

};