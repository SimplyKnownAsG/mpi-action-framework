%module(directors="1", docstring="This is the MAF's docstring!") maf

%include "std_string.i"
%include "exception.i"
%include "std_vector.i"
%include "std_shared_ptr.i"
%feature("autodoc", "2");

%{
#include "maf/maf.hpp"
%}

namespace std {
    %template(StringVector) vector<string>;
}

%exception {
    try {
        $action
    } catch(maf::Exception *ex) {
        std::ostringstream full_msg;
        full_msg <<  typeid(ex).name() << " in $decl: " << ex->what();
        SWIG_exception(SWIG_RuntimeError, full_msg.str().c_str());
    } catch(std::exception& ex) {
        std::ostringstream full_msg;
        full_msg <<  typeid(ex).name() << " in $decl: " << ex.what();
        SWIG_exception(SWIG_RuntimeError, full_msg.str().c_str());
    } catch(std::exception *ex) {
        std::ostringstream full_msg;
        full_msg <<  typeid(ex).name() << " in $decl: " << ex->what();
        SWIG_exception(SWIG_RuntimeError, full_msg.str().c_str());
    } catch(std::string &msg) {
        std::string full_msg = "something bad happend in $decl: " + msg;
        SWIG_exception(SWIG_RuntimeError, full_msg.c_str());
    } catch(...) {
        SWIG_exception(SWIG_RuntimeError, "something bad happened in $decl, but I do not know what it was :-(");
    }
};

%feature("director") maf::Context;
%shared_ptr(maf::Context);
%feature("director") maf::Action;
%feature("nodirector") maf::Action::start;
%shared_ptr(maf::MafComm);
%shared_ptr(maf::Action);
%template(ActionVector) std::vector<std::shared_ptr<maf::Action>>;
%shared_ptr(maf::Exception);
%shared_ptr(maf::EndLoopAction);
%shared_ptr(maf::EmptyAction);
%shared_ptr(maf::SplitMpiAction);
%feature("director") maf::ActionFactory;
%shared_ptr(maf::ActionFactory);
%shared_ptr(maf::Controller);
%shared_ptr(maf::BcastController);
%shared_ptr(maf::ScatterController);
%feature("director") maf::Controller;
%feature("director") maf::BcastController;
%feature("director") maf::ScatterController;
%feature("nodirector") maf::Controller::scatter;
%feature("nodirector") maf::BcastController::scatter;
%feature("nodirector") maf::ScatterController::scatter;
%feature("nodirector") maf::Controller::bcast;
%feature("nodirector") maf::BcastController::bcast;
%feature("nodirector") maf::ScatterController::bcast;
%shared_ptr(maf::Archive);
%shared_ptr(maf::ReadArchive);
%shared_ptr(maf::WriteArchive);



%include "maf/maf.hpp"

%template(log) maf::log<std::string>;
%template(warning) maf::warning<std::string>;
%template(barrier) maf::barrier<std::string>;


%pythoncode %{

_FACTORY_REFRENCES = []

_ACTION_STACK = set()

class Action(_maf.Action):

    def __init__(self):
        _maf.Action.__init__(self)
        _ACTION_STACK.add(self)
    
    def __del__(self):
        # when everything is getting cleaned up, it is possible that the _ACTION_STACK is None
        if _ACTION_STACK is not None:
            _ACTION_STACK.discard(self)

    def type_name(self):
        return self.__class__.__name__

def action(klass):
    class PyActionFactory(ActionFactory):
        
        def create_action(self):
            return klass()
    
    factory = PyActionFactory(klass.__name__)
    ActionFactory.Register(factory)
    _FACTORY_REFRENCES.append(factory)
    return klass

%}
