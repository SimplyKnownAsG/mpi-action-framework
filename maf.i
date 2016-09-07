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

%feature("director") Thing;
%feature("director") maf::Action;
%feature("director") maf::ActionFactory;
%shared_ptr(maf::ActionFactory);
%feature("director") maf::Controller;
%feature("director") maf::BcastController;

%include "maf/maf.hpp"


%pythoncode %{

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
            mpi_print('calling PyActionFactory for {}'.format(klass))
            return klass()
    
    factory = PyActionFactory(klass.__name__)
    Action.Register(factory)

%}
