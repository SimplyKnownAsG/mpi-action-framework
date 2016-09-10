#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : public Controller {

    public:

        BcastController();

        std::shared_ptr<Action> distribute(std::shared_ptr<Action> action=NULL);

        std::shared_ptr<Action> scatter(std::vector<std::shared_ptr<Action>> actions);

    };

}