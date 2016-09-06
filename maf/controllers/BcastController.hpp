#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : public Controller {

    public:

        Action* distribute(Action* action=NULL);

    };

}