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
    } catch(std::exception& ex) {
        std::ostringstream full_msg;
        full_msg <<  typeid(ex).name() << " in $decl: " << ex.what();
        SWIG_exception(SWIG_RuntimeError, full_msg.str().c_str());
    } catch(std::string& msg) {
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
%shared_ptr(maf::Action);
%feature("director") maf::AbstractTestAction;
%shared_ptr(maf::AbstractTestAction);
%shared_ptr(maf::MafComm);
%shared_ptr(maf::Exception);
%shared_ptr(maf::EndLoopAction);
%shared_ptr(maf::EmptyAction);
%shared_ptr(maf::SplitMpiAction);
%shared_ptr(maf::RevertSplitAction);
%feature("director") maf::ActionFactory;
%shared_ptr(maf::ActionFactory);

%shared_ptr(maf::Controller);
%shared_ptr(maf::BcastController);
%shared_ptr(maf::ScatterController);
%shared_ptr(maf::TestController);
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

/* vectors */
%template(ActionVector) std::vector<std::shared_ptr<maf::Action>>;

%include "maf/maf.hpp"

%template(log) maf::log<std::string>;
%template(warning) maf::warning<std::string>;
%template(barrier) maf::barrier<std::string>;

%template(bcast) maf::MafComm::bcast<std::string>;
%template(bcast) maf::MafComm::bcast<long long>;
%template(bcast) maf::MafComm::bcast<double>;

%template(bcast) maf::MafComm::bcast<std::vector<long long>>;
%template(bcast) maf::MafComm::bcast<std::vector<double>>;


%template(assert_equal) maf::AbstractTestAction::assert_equal<std::string>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<int>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<long long>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<double>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<float>;

%template(assert_equal) maf::AbstractTestAction::assert_equal<std::vector<std::string>>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<std::vector<int>>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<std::vector<long long>>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<std::vector<double>>;
%template(assert_equal) maf::AbstractTestAction::assert_equal<std::vector<float>>;


%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<std::string>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<int>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<long long>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<double>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<float>;

%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<std::vector<std::string>>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<std::vector<int>>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<std::vector<long long>>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<std::vector<double>>;
%template(assert_not_equal) maf::AbstractTestAction::assert_not_equal<std::vector<float>>;


%template(assert_close) maf::AbstractTestAction::assert_close<int, int>;
%template(assert_close) maf::AbstractTestAction::assert_close<long long, long long>;
%template(assert_close) maf::AbstractTestAction::assert_close<double, double>;
%template(assert_close) maf::AbstractTestAction::assert_close<float, float>;

%template(assert_close) maf::AbstractTestAction::assert_close<std::vector<int>, int>;
%template(assert_close) maf::AbstractTestAction::assert_close<std::vector<long long>, long long>;
%template(assert_close) maf::AbstractTestAction::assert_close<std::vector<double>, double>;
%template(assert_close) maf::AbstractTestAction::assert_close<std::vector<float>, float>;



%template(assert_not_close) maf::AbstractTestAction::assert_not_close<int>;
%template(assert_not_close) maf::AbstractTestAction::assert_not_close<long long>;
%template(assert_not_close) maf::AbstractTestAction::assert_not_close<double>;
%template(assert_not_close) maf::AbstractTestAction::assert_not_close<float>;


%pythoncode %{


_FACTORY_REFRENCES = []

class ActionFactory(_maf.ActionFactory):

    def __init__(self, klass):
        _maf.ActionFactory.__init__(self, klass.__name__)
        self.klass = klass
        _FACTORY_REFRENCES.append(self)
        _maf.ActionFactory.Register(self)

    def create_action(self):
        return self.klass()


_ACTION_STACK = set()

class _Action(object):

    def __init__(self):
        _ACTION_STACK.add(self)

    def __del__(self):
        # when everything is getting cleaned up, it is possible that the _ACTION_STACK is None
        if _ACTION_STACK is not None:
            _ACTION_STACK.discard(self)

    def type_name(self):
        return self.__class__.__name__
    
    def serialize(self, archive):
        pass


class Action(_Action, _maf.Action):

    def __init__(self):
        _Action.__init__(self)
        _maf.Action.__init__(self)


class TestAction(_Action, _maf.AbstractTestAction):

    def __init__(self):
        _Action.__init__(self)
        _maf.AbstractTestAction.__init__(self)


def action(klass_or_method):
    if isinstance(klass_or_method, type):
        klass = klass_or_method
    else:
        def run(self):
            klass_or_method()
        cls_dict = dict(run=run, __doc__=klass_or_method.__doc__)
        klass = type(klass_or_method.__name__, (Action, ), cls_dict) 

    factory = ActionFactory(klass)
    return klass


def test(klass_or_method):
    if isinstance(klass_or_method, type):
        klass = klass_or_method
    else:
        def run(self):
            klass_or_method()
        cls_dict = dict(run=klass_or_method, __doc__=klass_or_method.__doc__)
        klass = type(klass_or_method.__name__, (TestAction, ), cls_dict) 

    factory = ActionFactory(klass)
    return klass

%}
