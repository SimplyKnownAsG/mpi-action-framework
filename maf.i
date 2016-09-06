%module(directors="1", docstring="This is the MAF's docstring!") maf

%include "std_string.i"
%include "exception.i"
%include "std_vector.i"
%feature("autodoc", "2");

%{
#include "maf/maf.hpp"
%}

namespace std {
    %template(StringVector) vector<string>;
}

%feature("director") Thing;
%include "maf/maf.hpp"

%exception {
    try {
        $action
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


%pythoncode %{

class Thing(_maf.Thing):
    def __getstate__(self):
        return { 'index' : self.index, 'name': self.name}
    def __setstate__(self, state):
        self.__init__(state['name'])
        self.index = state['index']

# thank you internet...
# http://stackoverflow.com/questions/34445045/passing-python-functions-to-swig-wrapped-c-code
def register(klass):
    import ctypes
    def callback():
        instance = klass.__new__()
        return instance.this
    c_func = ctypes.CFUNCTYPE(None)(callback)
    func_pointer = ctypes.cast(c_func, ctypes.c_void_p)
    Action.Register(klass.__name__, func_pointer)

%}
