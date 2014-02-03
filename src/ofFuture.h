#pragma once

#include <string>

#include "Poco/ActiveMethod.h"
#include "Poco/ActiveResult.h"
#include "Poco/ActiveStarter.h"
#include "Poco/ActiveRunnable.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"

using std::string;

using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::ActiveResultHolder;
using Poco::ActiveStarter;
using Poco::ActiveRunnable;
using Poco::ActiveRunnableBase;
using Poco::AutoPtr;
using Poco::Exception;
using Poco::NoThreadAvailableException;
using Poco::Thread;
using Poco::ThreadPool;

#define ofFuture ActiveResult

// this is an overriden ActiveMethod that allows us to catch exceptions in the
// activity starter class.  this should probably be in the poco core.
// ref: http://pocoproject.org/forum/viewtopic.php?f=12&t=5764
// note that by default, we are requiring the use of our implementation of the
// active starter, (OwnerType)
template <class ResultType, class ArgType, class OwnerType, class StarterType = OwnerType>
class ofFutureMethod: public ActiveMethod <ResultType, ArgType, OwnerType, StarterType>
{
public:
    typedef ResultType (OwnerType::*Callback)(const ArgType&);
    
    ofFutureMethod(OwnerType* pOwner, Callback method) :
    ActiveMethod <ResultType, ArgType, OwnerType, OwnerType> (pOwner,method)
    { }

    virtual ~ofFutureMethod() { }
    
    // Invokes the ActiveMethod.
    ActiveResult<ResultType> operator () (const ArgType& arg) {
        ActiveResult<ResultType> result(new ActiveResultHolder<ResultType>());
        try {
            result = ActiveMethod <ResultType, ArgType, OwnerType, OwnerType>::operator()(arg);
            return result;
        } catch(Exception& exc) {
            result.error(exc);
            return result;
        } catch(std::exception& e) {
            result.error(e.what());
            return result;
        } catch( ... ) {
            result.error("unknown exception");
            return result;
        }
	}
};

// would prefer for this to simply subclass ofFutureMethod,
template <class ResultType, class OwnerType, class StarterType = OwnerType >
class ofFutureMethodVoid : public ActiveMethod <ResultType, void, OwnerType, StarterType> {
public:
	typedef ResultType (OwnerType::*Callback)(void);
    
    ofFutureMethodVoid(OwnerType* pOwner, Callback method) :
    ActiveMethod <ResultType, void, OwnerType, OwnerType> (pOwner,method)
    { }
    
    virtual ~ofFutureMethodVoid() { }
    
    // Invokes the ActiveMethod.
    ActiveResult <ResultType> operator () ()
    {
        ActiveResult<ResultType> result(new ActiveResultHolder<ResultType>());

        try
        {
            result = ActiveMethod<ResultType,void,OwnerType,OwnerType>::operator()();
            return result;
        }
        catch(Exception& exc)
        {
            result.error(exc);
            return result;
        }
        catch(std::exception& e)
        {
            result.error(e.what());
            return result;
        }
        catch( ... )
        {
            result.error("unknown exception");
            return result;
        }
	}
};


template<class ResultType,class ArgType>
class ofBaseFutureHandler
{
public:
    
//    typedef AutoPtr<ofBaseFutureHandler> Ptr;

    // constructor
    ofBaseFutureHandler(ThreadPool& _threadPool = ThreadPool::defaultPool(),
                        Thread::Priority _priority = Thread::PRIO_NORMAL,
                        const string& _name = "default") :
    threadPool(_threadPool),
    execute(this, &ofBaseFutureHandler::executeImpl),
    priority(_priority),
    name(_name)
    { }

    virtual ~ofBaseFutureHandler() { }
    
    // this is our activity starter
    static void start(ofBaseFutureHandler <ResultType, ArgType>* pOwner, ActiveRunnableBase::Ptr pRunnable) {
        pOwner->threadPool.startWithPriority(pOwner->priority,*pRunnable,pOwner->name);
        pRunnable->duplicate(); // The runnable will release itself.
	}

    // this is our active method.  activity starter is this class iteself.
    // the only difference between the default activity starter and our activity starter
    // is that our activity starter allows for a custom thread pool, thread names and
    // thread init priority
    ofFutureMethod <ResultType, ArgType, ofBaseFutureHandler <ResultType, ArgType> > execute;

protected:
    virtual ResultType executeImpl(const ArgType& input) = 0;

    Thread::Priority priority;
    string name;

    ThreadPool& threadPool;
};


template<class ResultType>
class ofBaseFutureHandler <ResultType, void>
{
public:
    
//    typedef AutoPtr<ofBaseFutureHandler> Ptr;

    // constructor
    ofBaseFutureHandler(ThreadPool& _threadPool = ThreadPool::defaultPool(),
                        Thread::Priority _priority = Thread::PRIO_NORMAL,
                        const string& _name = "default") :
    threadPool(_threadPool), execute(this, &ofBaseFutureHandler::executeImpl), priority(_priority), name(_name)
    { }
    
    virtual ~ofBaseFutureHandler() { }
    
    // this is our activity starter
    static void start(ofBaseFutureHandler <ResultType, void>* pOwner, ActiveRunnableBase::Ptr pRunnable) {
        pOwner->threadPool.startWithPriority(pOwner->priority,*pRunnable,pOwner->name);
        pRunnable->duplicate(); // The runnable will release itself.
	}
    
    // this is our active method.  activity starter is this class iteself.
    // the only difference between the default activity starter and our activity starter
    // is that our activity starter allows for a custom thread pool, thread names and
    // thread init priority
    ofFutureMethodVoid <ResultType, ofBaseFutureHandler <ResultType, void> > execute;
    
protected:
    virtual ResultType executeImpl(void) = 0;
    
    Thread::Priority priority;
    string name;
    
    ThreadPool& threadPool;
};
