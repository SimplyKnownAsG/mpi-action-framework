#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : public Controller {

    public:
    
        BcastController();

        std::shared_ptr<Action> bcast(std::shared_ptr<Action> action=NULL);

        std::shared_ptr<Action> scatter(std::vector<std::shared_ptr<Action>> actions);

    private:

        std::shared_ptr<Action> _default_share(std::shared_ptr<Action> action=NULL) override;

    };

}
