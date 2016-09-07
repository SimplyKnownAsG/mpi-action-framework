#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : public Controller {

    public:

        std::shared_ptr<Action> distribute(std::shared_ptr<Action> action=NULL);

    };

}