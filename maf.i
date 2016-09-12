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

%feature("director") maf::Action;
%shared_ptr(maf::Action);
%template(ActionVector) std::vector<std::shared_ptr<maf::Action>>;
%shared_ptr(maf::Exception);
%shared_ptr(maf::EndLoopAction);
%feature("director") maf::ActionFactory;
%shared_ptr(maf::ActionFactory);
%feature("director") maf::Controller;
%feature("nodirector") maf::Controller::distribute;
%feature("nodirector") maf::Controller::scatter;
%feature("director") maf::BcastController;
%feature("nodirector") maf::BcastController::distribute;
%feature("nodirector") maf::BcastController::scatter;
%shared_ptr(maf::Archive);
%shared_ptr(maf::ReadArchive);
%shared_ptr(maf::WriteArchive);

%include "maf/maf.hpp"


%pythoncode %{

_FACTORY_REFRENCES = []

_ACTION_STACK = []

class Action(_maf.Action):

    def tear_down(self):
        _ACTION_STACK.pop()
    
    def type_name(self):
        return self.__class__.__name__

# thank you internet...
# http://stackoverflow.com/questions/34445045/passing-python-functions-to-swig-wrapped-c-code
def register(klass):
    # import ctypes
    # def callback():
    #     instance = klass.__new__()
    #     return instance.this
    # c_func = ctypes.CFUNCTYPE(None)(callback)
    # func_pointer = ctypes.cast(c_func, ctypes.c_void_p)
    # Action.Register(klass.__name__, func_pointer)
    class PyActionFactory(ActionFactory):
        
        def create_action(self):
            _ACTION_STACK.append(klass())
            return _ACTION_STACK[-1]
    
    factory = PyActionFactory(klass.__name__)
    ActionFactory.Register(factory)
    _FACTORY_REFRENCES.append(factory)

%}
