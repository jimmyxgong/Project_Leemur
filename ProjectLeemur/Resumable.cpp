#include "Resumable.h"


//
//Resumable Resumable::promise_type::getReturnObject() 

//auto Resumable::promise_type::initial_suspend() {
//	return suspend_never{};
//}
//
//auto Resumable::promise_type::final_suspend() {
//	return suspend_never{};
//}


//void Resumable::resume() const {
//	coroutine.resume();
//}


//Resumable::Resumable(const Resumable && rhs) :
//	coroutine(rhs.coroutine)
//{
//	//rhs.coroutine = nullptr;
//}

//Resumable & Resumable::operator=(Resumable const && rhs) {
//	if (&rhs != this) {
//		coroutine = rhs.coroutine;
//		//rhs.coroutine = nullptr;
//	}
//
//	return *this;
//}